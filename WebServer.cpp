#include "general.h"
#include "WebServer.h"


WebServer::WebServer() {
}

WebServer::~WebServer() {
}

void WebServer::startServer() {
	if (WSAStartup(MAKEWORD(2, 2), &wData) == 0)
	{
		printf("WSA Startup succes\n");
	}
	SOCKADDR_IN addr;
	int addrl = sizeof(addr);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	this_s = socket(AF_INET, SOCK_STREAM, NULL);
	if (this_s == SOCKET_ERROR) {
		printf("Socket not created\n");
	}

	if (bind(this_s, (struct sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR) {
		printf("Socket succed binded\n");
	}

	if (listen(this_s, SOMAXCONN) != SOCKET_ERROR) {
		printf("Start listenin at port%u\n", ntohs(addr.sin_port));
	}
	handle();
}

void WebServer::closeServer() {
	closesocket(this_s);
	WSACleanup();
	std::cout << "Server was stoped. You can close app" << std::endl;
}


void WebServer::handle() {
	while (true)
	{
		SOCKET acceptS;
		SOCKADDR_IN addr_c;
		int addrlen = sizeof(addr_c);
		if ((acceptS = accept(this_s, (struct sockaddr*)&addr_c, &addrlen)) != 0) {
			printf("send\n");
			printf("sended Client connected from 0  %u.%u.%u.%u:%u\n",
				(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b1,
				(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b2,
				(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b3,
				(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b4,
				ntohs(addr_c.sin_port));
			WebClient* client = new WebClient(acceptS, addr_c);

		}
		Sleep(50);
	}
}