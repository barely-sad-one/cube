#pragma once

#include <string.h>

namespace cube
{

constexpr usize string8_length(const char *cstr)
{
  if (utility::is_const_evaluated())
  {
    usize length = 0;
    while (*cstr != '\0')
    {
      ++cstr;
      ++length;
    }
    return length;
  } else
  {
    return strlen(cstr);
  }
}

class StringView
{
  const char *data;
  usize length;

  constexpr StringView(const char *cstr)
    : data(cstr), length(string8_length(cstr)) {}
};

struct String8
{
  char  *data;
  usize len;
};

}
