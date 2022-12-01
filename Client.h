#pragma once
#include "general.h"
#include "Request.h"
#include "Response.h"

namespace server
{
	enum class STATES
	{
		READING,
		READ,
		WRITING,
		WRITTEN,
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
		bool Continue();
		void Handle();
		void SetState(const STATES newState);
		STATES GetState() const;
		const struct epoll_event GetEvent() const;

	private:
		SOCKET connected_socket;
		STATES state;
		Request request;
		Response response;
		struct epoll_event event;

		CODES ContinueReading();
		CODES ContinueWriting();
		CODES InitRead();
	};
}
