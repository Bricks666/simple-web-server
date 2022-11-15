#include "Server.h"
#include "Client.h"

using namespace server;

Server::Server()
{
}

Server::~Server()
{
}

int Server::Start()
{
	int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);

	if (result)
	{
		Close();
		return -1;
	}

	SOCKADDR_IN addrInfo;
	addrInfo.sin_addr.S_un.S_addr = INADDR_ANY;
	addrInfo.sin_port = htons(PORT);
	addrInfo.sin_family = AF_INET;

	connect_socket = socket(addrInfo.sin_family, SOCK_STREAM, IPPROTO_TCP);
	SET_NONBLOCK(connect_socket);

	if (connect_socket == INVALID_SOCKET || connect_socket == SOCKET_ERROR)
	{
		Close();
		cout << "Invalid socket" << endl;
		return -1;
	}

	result = bind(connect_socket,
				  reinterpret_cast<struct sockaddr *>(&addrInfo),
				  sizeof(addrInfo));
	if (result == SOCKET_ERROR)
	{
		Close();
		cout << "Invalid bind" << endl;
		return -1;
	}

	result = listen(connect_socket, SOMAXCONN);
	if (result == SOCKET_ERROR)
	{
		Close();
		cout << "Invalid listen" << endl;
		return -1;
	}

	epoll = epoll_create(1);
	if (epoll == -1)
	{
		cout << "Error creation epoll" << endl;
		return 0;
	}

	listenEvent.data.fd = connect_socket;
	listenEvent.events = EPOLLIN | EPOLLET;

	epoll_ctl(epoll, EPOLL_CTL_ADD, connect_socket, &listenEvent);

	Handle();
}

void Server::Close()
{
	closesocket(connect_socket);
	connect_socket = INVALID_SOCKET;
	WSACleanup();
	cout << "Server has been stopped\n"
		 << WSAGetLastError() << endl;
}

void Server::Handle()
{
	while (true)
	{
		events.resize(clients.size() + 1);
		int count = epoll_wait(epoll, &events[0], events.size(), 5000);
		cout << count << " count" << endl;
		if (count == -1)
		{
			continue;
		}

		callback(count);
	}
}

void Server::callback(const int count)
{
	for (int i = 0; i < count; i++)
	{
		SOCKET hSocketIn = events[i].data.fd;

		if (listenEvent.data.fd == (int)hSocketIn)
		{
			if (!events[i].events == EPOLLIN)
				continue;

			struct sockaddr_in socket_address;
			size_t socket_address_len = sizeof(socket_address);
			const SOCKET socket_descriptor = accept(hSocketIn, (struct sockaddr *)&socket_address, (int *)&socket_address_len);

			if (socket_descriptor != INVALID_SOCKET)
			{
				clients[socket_descriptor] = shared_ptr<Client>(new Client(socket_descriptor));

				auto it = clients.find(socket_descriptor);
				if (it == clients.end())
					continue;

				struct epoll_event event = it->second->GetEvent();
				epoll_ctl(epoll, EPOLL_CTL_ADD, it->first, &event);
			}
			continue;
		}

		auto client = clients.find(events[i].data.fd);
		if (client == clients.end())
			continue;

		if (!client->second->Continue())
		{
			epoll_ctl(epoll, EPOLL_CTL_DEL, client->first, NULL);
			clients.erase(client);
		}
	}
}

void Server::addClient(const SOCKET hostSocket, struct epoll_event &event)
{
	if (listenEvent.data.fd == hostSocket)
	{
		if (event.events != EPOLLIN)
		{
			return;
		}
		struct sockaddr_in clientSocketAddr;
		size_t clientLen = sizeof(clientSocketAddr);

		const SOCKET clientSocket = accept(hostSocket, (struct sockaddr *)&clientSocketAddr, (int *)&clientLen);

		if (clientSocket != INVALID_SOCKET)
		{
			clients[hostSocket] = shared_ptr<Client>(new Client(clientSocket));

			auto client = clients.find(hostSocket);
			if (client == clients.end())
			{
				return;
			}
			struct epoll_event event = client->second->GetEvent();
			epoll_ctl(epoll, EPOLL_CTL_ADD, client->first, &event);
		}
	}
}
