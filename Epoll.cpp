#include "Epoll.h"
#include <map>
#ifndef WIN32
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#else
#include <iostream>
#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

using namespace std;

map<int, epoll_event> epollMapSockets;

int epoll_create(int size)
{
	return 1;
}

int epoll_ctl(int epfd, int op, int fd, struct epoll_event* event)
{
	switch (op)
	{
	case EPOLL_CTL_ADD:
	case EPOLL_CTL_MOD:
		epollMapSockets[fd] = *event;
		return 0;
	case EPOLL_CTL_DEL:
		if (epollMapSockets.find(fd) == epollMapSockets.end())
			return -1;

		epollMapSockets.erase(fd);
		return 0;
	}
	return 0;
}

int epoll_wait(int epfd, struct epoll_event* events, int maxevents, int timeout)
{
	if ((!events) || (!maxevents))
		return -1;

	//Создаем и обнуляем структуры для функции select
	fd_set readfds, writefds, exceptfds;

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&exceptfds);

	//Заполняем структуры сокетами
	int nFDS = 0;
	for (auto client = epollMapSockets.begin(); client != epollMapSockets.end(); ++client)
	{
		if (client->first == -1)
			continue;

		if (client->first > nFDS)
			nFDS = client->first;

		FD_SET(client->first, &readfds);
		FD_SET(client->first, &writefds);
		FD_SET(client->first, &exceptfds);
	}

	//Задаем интервал ожидания
	struct timeval tv;
	tv.tv_sec = timeout / 1000;
	tv.tv_usec = timeout - tv.tv_sec * 1000;

	int error = select(nFDS + 1, &readfds, &writefds, &exceptfds, &tv);

	if (error == -1) {
		return error;
	}

	int nRetEvents = 0;
	for (auto client = epollMapSockets.begin(); (client != epollMapSockets.end() && nRetEvents < maxevents); ++client)
	{
		if (client->first == -1)
			continue;
		
		if (!FD_ISSET(client->first, &readfds) && !FD_ISSET(client->first, &writefds) && !FD_ISSET(client->first, &exceptfds))
			continue;

		memcpy(&events[nRetEvents].data, &client->second.data, sizeof(epoll_data));

		if (FD_ISSET(client->first, &readfds))
			events[nRetEvents].events |= EPOLLIN;
		if (FD_ISSET(client->first, &writefds))
			events[nRetEvents].events |= EPOLLOUT;
		if (FD_ISSET(client->first, &exceptfds))
			events[nRetEvents].events |= EPOLLERR;

		nRetEvents++;
	}

	return nRetEvents;
}