#pragma once
#include "general.h"
#include "File.h"

namespace server
{
  class Response
  {
  public:
    string answer;
    string MakeAnswer();
    map<string, string> headers;
    static map<string, string> content_type;
    static map<string, string> response_name;
  private:
    static string MakeHeader(map<string, string> header);
  };

}
