#pragma once
#include "general.h"

namespace server
{
  class Response
  {
  public:
    int code = 200;
    string redirect_URL = "";
    string body = "";
    string answer;
    string MakeAnswer();
  private:
    static map<int, string> response_name;
    static string GetHeader(const string name, const string value, const string separator, const string end);
  };
}
