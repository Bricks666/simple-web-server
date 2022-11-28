#include "Server.h"
#include "Client.h"

using namespace server;

int Server::Start()
{
  int result;

#ifdef _WIN32
  result = WSAStartup(MAKEWORD(2, 2), &wsa_data);

  if (result)
  {
    Close();
    return -1;
  }
#endif

  sockaddr_in address_info;
  address_info.sin_addr.s_addr = INADDR_ANY;
  address_info.sin_port = htons(PORT);
  address_info.sin_family = AF_INET;

  connect_socket = socket(address_info.sin_family, SOCK_STREAM, IPPROTO_TCP);
  SET_NONBLOCK(connect_socket);

  if (connect_socket == INVALID_SOCKET)
  {
    Close();
    cout << "Invalid socket" << endl;
    return -1;
  }

  result = bind(connect_socket,
                reinterpret_cast<struct sockaddr *>(&address_info),
                sizeof(address_info));
  if (result == INVALID_SOCKET)
  {
    Close();
    cout << "Invalid bind" << endl;
    return -1;
  }

  result = listen(connect_socket, SOMAXCONN);
  if (result == INVALID_SOCKET)
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

  listen_event.data.fd = connect_socket;
  listen_event.events = EPOLLIN | EPOLLET;

  epoll_ctl(epoll, EPOLL_CTL_ADD, connect_socket, &listen_event);

  Handle();

  return 0;
}

void Server::Close()
{
  closesocket(connect_socket);
  connect_socket = INVALID_SOCKET;
#ifdef _WIN32
  WSACleanup();
  cout << "Server has been stopped\n"
       << WSAGetLastError() << endl;
#endif
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

    Callback(count);
  }
}

void Server::Callback(const int count)
{
  for (int i = 0; i < count; i++)
  {
    SOCKET input_socket = events[i].data.fd;

    if (listen_event.data.fd == input_socket)
    {
      if (!events[i].events == EPOLLIN)
        continue;

      struct sockaddr_in socket_address;
      socklen_t socket_address_len = sizeof(socket_address);

#ifdef _WIN32
      const SOCKET socket_descriptor = accept(input_socket, (struct sockaddr *)&socket_address, (int *)&socket_address_len);
#else
      const SOCKET socket_descriptor = accept(input_socket, (struct sockaddr *)&socket_address, &socket_address_len);
#endif

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

void Server::AddClient(const SOCKET host_socket, struct epoll_event &event)
{
  if (listen_event.data.fd == host_socket)
  {
    if (event.events != EPOLLIN)
    {
      return;
    }
    struct sockaddr_in client_socket_address;
    socklen_t client_length = sizeof(client_socket_address);

#ifdef _WIN32
    const SOCKET client_socket = accept(host_socket, (struct sockaddr *)&client_socket_address, (int *)&client_length);
#else
    const SOCKET client_socket = accept(host_socket, (struct sockaddr *)&client_socket_address, &client_length);
#endif

    if (client_socket == INVALID_SOCKET)
    {
      return;
    }

    clients[host_socket] = shared_ptr<Client>(new Client(client_socket));

    auto client = clients.find(host_socket);
    if (client == clients.end())
    {
      return;
    }
    struct epoll_event event = client->second->GetEvent();
    epoll_ctl(epoll, EPOLL_CTL_ADD, client->first, &event);
  }
}
