#pragma once
#include "general.h"
#include "WebClient.h"

WebClient::WebClient(SOCKET s, SOCKADDR_IN sock_in)
{

	c_sock = s;
	c_addr = sock_in;
	printf("Client created\n");
	handle();
}


WebClient::~WebClient()
{
}

void WebClient::handle()
{
	while (true)
	{
		int k = recv(c_sock, buffer, sizeof(buffer), NULL);
		if (k > 0) {
			printf(buffer);
		}
		Sleep(30);
	}
}