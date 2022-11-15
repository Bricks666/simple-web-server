#include "Client.h"
#include "Response.h"

using namespace server;

Client::Client() {

}

Client::Client(SOCKET s) {
	connectSocket = s;
	state = STATES::ACCEPTED;
	event.data.fd = s;
	event.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLOUT;
	cout << "Client created" << endl;
}

Client::~Client() {
	if(connectSocket != INVALID_SOCKET) {
		closesocket(connectSocket);
	}
	cout << "Client destroyed" << endl;
}

void Client::SetState(const STATES newState) {
	state = newState;
}

STATES Client::GetState() const {
	return state;
}

bool Client::Continue() {
 	if (connectSocket == INVALID_SOCKET) {
		return false;
	}

	switch (GetState())
	{
	case STATES::ACCEPTED:
		SetState(STATES::READING);
		break;
	case STATES::READED:
		switch (InitRead())
		{
		case CODES::READY_CODE:
			SetState(STATES::WRITTING);
			break;
		case CODES::ERROR_CODE: 
			return false;

		}
		return true;
	case STATES::READING:

		switch (ContinueReading())
		{
		case CODES::READY_CODE:
			SetState(STATES::READED);
			break;
		case CODES::ERROR_CODE:
			return false;
		}

		return true;
	case STATES::WRITTING:
		switch (ContinueWritting())
		{
		case CODES::READY_CODE:
			SetState(STATES::WRITED);
			break;
		case CODES::ERROR_CODE:
			return false;
		}

		return true;
	case STATES::WRITED:
		return false;
	default:
		return false;
	}

	return true;

}

CODES Client::ContinueReading() {
	char buffer[4096];

	int writed = recv(connectSocket, buffer, sizeof(buffer), NULL);
	if (writed >= 0) {
		received.resize(received.size() + writed);
		memcpy(&received[received.size() - writed], buffer, writed);
		cout << received.data() << endl;;

		const std::string strInputString((const char*)&received[0]);
		if (strInputString.find("\r\n\r\n") != -1)
			return CODES::READY_CODE;

		return CODES::WAIT_CODE;
	}

	return CODES::ERROR_CODE;
}

CODES Client::ContinueWritting() {
	int result = send(connectSocket, &sended[0], sended.size(), NULL);
	
	if (result == sended.size()) {
		return CODES::READY_CODE;
	}

	if (result > 0) {
		vector<char> temp(sended.size() - result);
		memcpy(&temp[0], &sended[result], sended.size() - result);
		sended = temp;

		return CODES::WAIT_CODE;

	}

	return CODES::ERROR_CODE;
}

CODES Client::InitRead() {
	string document = Response::get_document("./document/index.html");
	string answer = Response::get_answer(document);
	sended.resize(answer.length());
	memcpy(&sended[0], answer.c_str(), answer.length());

	return CODES::READY_CODE;
}

const struct epoll_event Client::GetEvent() const {
	return event;
}