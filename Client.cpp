#include "Client.h"
#include "File.h"

using namespace server;

Client::Client()
{
}

Client::Client(SOCKET s)
{
  connected_socket = s;
  state = STATES::ACCEPTED;
  event.data.fd = s;
  event.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLOUT;
  cout << "Client created" << endl;
}

Client::~Client()
{
  if (connected_socket != INVALID_SOCKET)
  {
    closesocket(connected_socket);
  }
  cout << "Client destroyed" << endl;
}

void Client::SetState(const STATES newState)
{
  state = newState;
}

STATES Client::GetState() const
{
  return state;
}

void Client::Handle() {
    while (Continue()) {
        cout << "Work " << connected_socket << endl;
    }
}

bool Client::Continue()
{
  if (connected_socket == INVALID_SOCKET)
  {
    return false;
  }

  switch (GetState())
  {
  case STATES::ACCEPTED:
    SetState(STATES::READING);
    break;
  case STATES::READ:
    switch (InitRead())
    {
    case CODES::READY_CODE:
      SetState(STATES::WRITING);
      break;
    case CODES::ERROR_CODE:
      return false;
    }
    return true;
  case STATES::READING:

    switch (ContinueReading())
    {
    case CODES::READY_CODE:
      SetState(STATES::READ);
      break;
    case CODES::ERROR_CODE:
      return false;
    }

    return true;
  case STATES::WRITING:
    switch (ContinueWriting())
    {
    case CODES::READY_CODE:
      SetState(STATES::WRITTEN);
      break;
    case CODES::ERROR_CODE:
      return false;
    }

    return true;
  case STATES::WRITTEN:
    return false;
  default:
    return false;
  }

  return true;
}

CODES Client::ContinueReading()
{
  char buffer[4096];

  const int written = recv(connected_socket, buffer, sizeof(buffer), NULL);
  if (written > 0)
  {
    const string string_request(buffer, written);
    request.ParseRequest(string_request);
    cout << string_request << endl;

    if (string_request.find("\r\n\r\n") != -1)
      return CODES::READY_CODE;

    return CODES::WAIT_CODE;
  }

  return CODES::ERROR_CODE;
}

CODES Client::ContinueWriting()
{
  const int result = send(connected_socket, &response.answer[0], response.answer.size(), NULL);

  if (result == response.answer.size())
  {
    return CODES::READY_CODE;
  }

  if (result > 0)
  {
    string temp;
    memcpy(&temp[0], &response.answer[result], response.answer.size() - result);
    response.answer = temp;

    return CODES::WAIT_CODE;
  }

  return CODES::ERROR_CODE;
}

CODES Client::InitRead()
{
  const string path = request.GetURL();
  const string fullpath = (path != "/" && path != "" ? path : "/index.html");
  const string document = File::GetDocument("./document" + fullpath);
  if (document == "")
  {
      response.headers["Code"] = "301";
      response.headers["Location"] = "/error.html";
  }
  else
  {
      response.headers["Body"] = document;
      response.headers["Content-Lenght"] = document.length();
      response.headers["Code"] = "200";
  }
  response.MakeAnswer();

  return CODES::READY_CODE;
}

const struct epoll_event Client::GetEvent() const
{
  return event;
}
