#pragma once
#include "general.h"
#include "Client.h"

namespace server
{

	class Server
	{

	public:
		Server();
		~Server();
		int Start();
		void Handle();
		void Close();

	private:
		WSAData wsaData;
		SOCKET connectSocket;
		map<SOCKET, shared_ptr<Client>> clients;
		struct epoll_event listenEvent;
		vector<struct epoll_event> events;
		int epoll;

		void callback(const int count);
		void addClient(const SOCKET hostSocket, struct epoll_event &event);
	};

}
