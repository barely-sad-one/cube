namespace cube
{

// FIXME: remove platform specific code
String16H str16H_from_8(StringView8 str)
{
  String16H result;
  int required = MultiByteToWideChar(CP_UTF8,
                                     0,
                                     str.data,
                                     static_cast<i32>(str.length),
                                     nullptr,
                                     0);

  result.data = allocate<wchar_t, MemoryType_String>(required + 1);
  MultiByteToWideChar(CP_UTF8,
                      0,
                      str.data,
                      static_cast<i32>(str.length),
                      result.data,
                      required);

  result.data[required] = L'\0';
  result.length = required;

  return result;
}

String16 str16_from_8(Arena *arena, StringView8 str)
{
  String16 result;
  int required = MultiByteToWideChar(CP_UTF8,
                                     0,
                                     str.data,
                                     static_cast<i32>(str.length),
                                     nullptr,
                                     0);

  result.data = arena->push<wchar_t>(required + 1);
  MultiByteToWideChar(CP_UTF8,
                      0,
                      str.data,
                      static_cast<i32>(str.length),
                      result.data,
                      required);

  result.data[required] = L'\0';
  result.length = required;

  return result;
}

String16H str16H_from_8(String8H str)
{
  StringView8 str_view(str.data, str.length);
  return str16H_from_8(str_view);
}

String16 str16_from_8(Arena *arena, String8 str)
{
  StringView8 str_view(str.data, str.length);
  return str16_from_8(arena, str_view);
}

}
