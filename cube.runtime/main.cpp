#if compiler_msvc

#define UNICODE
#define _UNICODE

#include <windows.h>
#include "entry.cpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
  (void) hInstance;
  (void) hPrevInstance;
  (void) lpCmdLine;
  (void) nCmdShow;

  return cube::entry();
}

#else

#include "entry.cpp"

int main(int argc, char* argv[])
{
  (void) argc;
  (void) argv;

  return cube::entry();
}

#endif // entry point
