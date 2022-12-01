#include "Response.h"

using namespace server;

string Response::GetHeader(const string name, const string value, const string separator = ":", const string end = ";")
{
  return name + separator + value + end + "\r\n";
}

string Response::MakeAnswer()
{
    const string code = headers["Code"];
    string stringify_answer;
    stringify_answer.append("HTTP/1.1 ").append(code).append(" Moved Permanently\r\n").append("charset=utf8;");

  switch (stoi(code))
  {
  case 200:
    stringify_answer
        .append(Response::GetHeader("Content-Type", "text/html", ":"))
        .append(Response::GetHeader("Content-Length", headers["Content-Length"]))
        .append(Response::GetHeader("\r\n", headers["Body"], "", ""));
    break;
  case 301:
    stringify_answer
        .append(Response::GetHeader("Location", headers["Location"], ":"));
    break;
  default:
    break;
  }

  answer = stringify_answer;

  cout  << "RESPONSE" << endl << answer << endl;

  return answer;
}
