#pragma once
#include "general.h"

namespace server
{
  class File
  {
  public:
    static string ReadFile(const string path);
    static string GetFileExtension(const string path);
  };
}
