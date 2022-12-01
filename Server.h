#pragma once
#include "general.h"
#include "Client.h"

namespace server
{

	class Server
	{

	public:
		int Start();
		void Handle();
		void Close();

	private:
    #ifdef _WIN32
		  WSAData wsa_data;
    #endif

		SOCKET connect_socket;
		map<SOCKET, shared_ptr<Client>> clients;
		struct epoll_event listen_event;
		vector<struct epoll_event> events;
		int epoll;

		void Callback(const int count);
		void AddClient(const SOCKET host_socket);
		void CloseSocket(const SOCKET host_socket);
	};

}
