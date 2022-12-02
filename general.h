#ifndef GENERAL
#define GENERAL

#define PORT 3001
#define HOME "./"

#ifndef _WIN32
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/epoll.h>
#else
#include <Winsock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include "epoll.h"
#pragma comment(lib, "ws2_32.lib")
#endif

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <memory.h>
#include <memory>
#include <vector>
#include <stdlib.h>
#include <thread>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define SET_NONBLOCK(socket)           \
  if (true)                            \
  {                                    \
    DWORD dw = true;                   \
    ioctlsocket(socket, FIONBIO, &dw); \
  }
#else
#include <fcntl.h>
#define SET_NONBLOCK(socket)                                              \
  if (fcntl(socket, F_SETFL, fcntl(socket, F_GETFL, 0) | O_NONBLOCK) < 0) \
    printf("error in fcntl errno=%i\n", errno);
#define closesocket close
#define SOCKET int
#define INVALID_SOCKET -1
#endif



using namespace std;

#endif
