#pragma once
#include "general.h"

namespace server

{
  class Request
  {
  public:
    string content;
    string GetURL();
  };
}
