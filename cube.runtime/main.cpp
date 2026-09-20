#include <windows.h>

#include "entry.cpp"

// entry point
#if compiler_msvc

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
  (void) hInstance;
  (void) hPrevInstance;
  (void) lpCmdLine;
  (void) nCmdShow;

  return cube::entry();
}

#else

int main(int argc, char* argv[])
{
  (void) argc;
  (void) argv;

  return cube::entry();
}

#endif // entry point
