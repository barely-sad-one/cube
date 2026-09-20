#include <string.h>

namespace cube::platform
{

static PlatformState g_platform_state;

PlatformState* get_platform_state()
{
  return &g_platform_state;
}

void* memory_copy(void *dest, const void *src, usize size)
{
  return memcpy(dest, src, size);
}

void* memory_move(void *dest, void *src, usize size)
{
  return memmove(dest, src, size);
}

void* memory_set(void *dest, i32 value, usize size)
{
  return memset(dest, value, size);
}

void* memory_zero(void *dest, usize size)
{
  return memory_set(dest, 0, size);
}

}
