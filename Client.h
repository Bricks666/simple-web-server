#pragma once
#include "general.h"
namespace server
{
	enum class STATES
	{
		READING,
		READED,
		WRITTING,
		WRITED,
		ACCEPTED
	};

	enum class CODES
	{
		ERROR_CODE,
		WAIT_CODE,
		READY_CODE
	};

	class Client
	{
	public:
		explicit Client();
		explicit Client(SOCKET s);
		~Client();
		void Handle();
		bool Continue();
		void SetState(const STATES newState);
		STATES GetState() const;
		const struct epoll_event GetEvent() const;

	private:
		SOCKET connectSocket;
		STATES state;
		vector<char> received;
		vector<char> sended;
		struct epoll_event event;

		CODES ContinueReading();
		CODES ContinueWritting();
		CODES InitRead();
	};
}
