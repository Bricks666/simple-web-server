#include "Response.h"

using namespace server;

string Response::MakeHeader(map<string, string> header)
{
    string response = "";
    response.append(header["key"]);
    response.append(header["separator"] == ""?  ":" : header["separator"]);
    response.append(header["value"]);
    response.append(header["end"] == "" ? "\r\n" : header["end"]);

    return response;
}

string Response::MakeAnswer()
{
    const string code = headers["Code"];
    string stringify_answer;
    stringify_answer.append("HTTP/1.1 ").append(code).append(" ").append(Response::response_name[code]).append("\r\n");
    const auto is_text_content = headers["Content-Type"].find("text") == 0;
  switch (stoi(code))
  {
  case 200:
      stringify_answer
          .append(Response::MakeHeader({ {"key", "Content-Type"}, {"value", headers["Content-Type"] }, {"end", is_text_content ? "; " : ""} }));
      if (is_text_content) {
          stringify_answer.append(Response::MakeHeader({ {"key", "charset"}, {"value", "utf-8"}, {"separator", "="} }));
      }
      stringify_answer.append(Response::MakeHeader({ {"key", "Content-Length"}, {"value", headers["Content-Length"] } }))
          .append("\r\n")
          .append(headers["Body"]);;
    break;
  case 301:
      stringify_answer
          .append(Response::MakeHeader({ {"key", "Location"}, {"value",headers["Location"] } }));
    break;
  default:
    break;
  }

  answer = stringify_answer;

  return answer;
}


map<string, string> Response::content_type = {
    {"html", "text/html"},
    {"htm", "text/html"},
    {"ico", "image/x-icon"},
    {"avif", "image/avif"},
    {"css", "text/css"},
    {"jpg", "image/jpeg"},
    {"jpeg", "image/jpeg"},
    {"svg", "image/svg+xml"},
    {"webp", "image/webp"},
    {"woof", "font/woof"},
    {"woof2", "font/woof2"},
    {"otf", "font/otf"},
    {"png", "image/png"},
    {"js", "text/javascrip"},
    {"mjs", "text/javascrip"},
};

map<string, string> Response::response_name = {
    {"200", "OK"},
    {"201", "Created"},
    {"301", "Moved Permanently"}
};