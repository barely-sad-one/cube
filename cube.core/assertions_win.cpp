#include <shlwapi.h>

namespace cube
{
  
void report_assertion_failure(const char *expr, const char *file, usize line, const char *message)
{
  wchar_t buffer[2048];
  wnsprintfW(buffer,
             2048,
             L"--- Assertion Failed ---\n\n"
             L"Expression: %S\n"
             L"File: %S\n"
             L"Line: %I64u\n\n"
             L"Message: %S\n",
             expr,
             file,
             static_cast<i64>(line),
             message ? message : "None");
  MessageBoxW(nullptr, buffer, L"Cube Engine - Fatal Error", MB_OK | MB_ICONERROR);
}

}
