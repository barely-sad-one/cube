#pragma once

#include <string.h>
#include <wchar.h>

namespace cube
{

template <typename T>
concept StringChar = utility::is_same_v<T, char> || utility::is_same_v<T, wchar_t>;

template <StringChar T = char>
constexpr usize string_length(const T *cstr)
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
  } else if constexpr (utility::is_same_v<T, char>)
  {
    return strlen(cstr);
  } else
  {
    return wcslen(cstr);
  }
}

template <StringChar T = char>
class StringView
{
public:
  const T *data;
  usize length;

  constexpr StringView(const T *cstr)
    : data(cstr), length(string_length<T>(cstr)) {}

  constexpr StringView(const T *cstr, usize length)
    : data(cstr), length(length) {}

  constexpr T operator[](usize idx) const
  {
    cube_assert_debug(idx < length);
    return data[idx];
  }

  constexpr bool operator==(const StringView& other) const
  {
    if (length != other.length) return false;
    if (data == other.data) return true;
    return platform::memory_compare<T>(data, other.data, length) == 0;
  }
};
using StringView8  = StringView<char>;
using StringView16 = StringView<wchar_t>;

template <StringChar T, bool ArenaSupport>
struct BasicString
{
  T     *data;
  usize length;

  void create(StringView<T> str_view) requires (!ArenaSupport)
  {
    data = allocate<T, MemoryType_String>(str_view.length + 1);
    platform::memory_copy<T>(data, str_view.data, str_view.length);
    data[str_view.length] = '\0';
    length = str_view.length;
  }

  void create(Arena *arena, StringView<T> str_view) requires ArenaSupport
  {
    data = arena->push<T>(str_view.length + 1);
    platform::memory_copy<T>(data, str_view.data, str_view.length);
    data[str_view.length] = '\0';
    length = str_view.length;
  }

  void destroy() requires (!ArenaSupport)
  {
    deallocate<T>(data);
    data = nullptr;
    length = 0;
  }
};
using String8H  = BasicString<char, false>;
using String8   = BasicString<char, true>;
using String16H = BasicString<wchar_t, false>;
using String16  = BasicString<wchar_t, true>;

template <StringChar T>
using StringH  = BasicString<T, false>;
template <StringChar T>
using String  = BasicString<T, true>;

template <StringChar T = char>
StringH<T> create_stringH(StringView<T> str_view)
{
  StringH<T> str;
  str.create(str_view);
  return str;
}

template <StringChar T = char>
String<T> create_string(Arena *arena, StringView<T> str_view)
{
  String<T> str;
  str.create(arena, str_view);
  return str;
}

template <StringChar T = char>
StringH<T> copy_stringH(StringH<T> str)
{
  StringView<T> str_view(str.data, str.length);
  StringH<T> s;
  s.create(str_view);
  return s;
}

template <StringChar T>
String<T> copy_string(Arena *arena, String<T> str)
{
  StringView<T> str_view(str.data, str.length);
  String<T> s;
  s.create(arena, str_view);
  return s;
}

cube_api String16H str16H_from_8(StringView8 str);
cube_api String16  str16_from_8(Arena *arena, StringView8 str);
cube_api String16H str16H_from_8(String8H str);
cube_api String16  str16_from_8(Arena *arena, String8 str);

}
