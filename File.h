#pragma once
#include "general.h"

namespace server
{
  class File
  {
  public:
    static string GetDocument(string path);
    static bool is_html(const string path);
  };
}
