#pragma once
#include "general.h"

namespace server
{
  class Response
  {
  public:
    string answer;
    string MakeAnswer();
    map<string, string> headers;
  private:
    static map<int, string> response_name;
    static string GetHeader(const string name, const string value, const string separator, const string end);
  };
}
