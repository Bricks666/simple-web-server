#pragma once
#ifndef GENERAL
#define GENERAL
#define WIN32_LEAN_AND_MEAN

#define PORT 3000

#ifndef WIN32
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/epoll.h>
#else
#include <Windows.h>
#include <Winsock2.h>
#include <winsock.h>
#include <WS2tcpip.h>
#include "epoll.h"
#pragma comment(lib, "ws2_32.lib")
#endif // !WIN32

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>

#ifdef WIN32
#define SET_NONBLOCK(socket)               \
	if (true)                              \
	{                                      \
		DWORD dw = true;                   \
		ioctlsocket(socket, FIONBIO, &dw); \
	}
#endif // WIN32

using namespace std;

#endif // !GENERAL
