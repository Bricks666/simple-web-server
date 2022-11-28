#pragma once
#include "general.h"

namespace server

{
  class Request
  {
  public:
    void ParseRequest(string _request);

    map<string, string> headers;
    const string& GetURL();
  private:
      string url;
      string request;
      static map<string, string> ParseHeaders(string& _request);
  };
}
