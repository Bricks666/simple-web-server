#include "Response.h"

using namespace server;

string Response::GetHeader(const string name, const string value, const string separator = ":", const string end = ";")
{
  return name + separator + value + end + "\r\n";
}

string Response::MakeAnswer()
{
  string stringify_answer;
  stringify_answer.append("HTTP/1.1 ").append(to_string(code)).append(" Moved Permanently\r\n").append(Response::GetHeader("charset", "utf-8", "="));

  switch (code)
  {
  case 200:
    stringify_answer
        .append(Response::GetHeader("Content-Type", "text/html", ":"))
        .append(Response::GetHeader("Content-Length", to_string(body.length())))
        .append(Response::GetHeader("\r\n", body, ""));
    break;
  case 301:
    stringify_answer
        .append(Response::GetHeader("Location", redirect_URL, ":", ""));
    break;
  default:
    break;
  }

  answer = stringify_answer;

  return answer;
}
