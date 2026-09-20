#if os_windows

#include <malloc.h>

namespace cube::platform
{

bool platform_initialize()
{
  SYSTEM_INFO sys_info;
  GetSystemInfo(&sys_info);

  PlatformState *state = get_platform_state();
  
  state->logical_processors = sys_info.dwNumberOfProcessors;
  state->allocation_granularity = sys_info.dwAllocationGranularity;
  state->page_size = sys_info.dwPageSize;
  state->large_page_size = GetLargePageMinimum();

  return true;
}

void platform_destroy()
{
  // nothing to do here!
}

align_t align_page_size()
{
  return get_platform_state()->page_size;
}

void* allocate(usize size, align_t alignment)
{
  cube_assert_debug(utility::is_pow2(alignment));
  return _aligned_malloc(size, alignment);
}

void* reallocate(void *ptr, usize new_size, align_t alignment)
{
  cube_assert_debug(utility::is_pow2(alignment));
  return _aligned_realloc(ptr, new_size, alignment);
}

void deallocate(void *ptr)
{
  cube_assert_debug(ptr != nullptr);
  _aligned_free(ptr);
}

void* reserve(usize size)
{
  cube_assert_debug((size & (platform::get_platform_state()->allocation_granularity - 1)) == 0);
  void *ptr = VirtualAlloc(nullptr, size, MEM_RESERVE, PAGE_NOACCESS);
  cube_assert_debug(ptr != nullptr);
  return ptr;
}

void release(void *ptr, usize /* size */)
{
  cube_assert_debug(ptr != nullptr);
  bool res = VirtualFree(ptr, 0, MEM_RELEASE);
  cube_assert_debug(res);
}

void commit(void *ptr, usize size)
{
  usize page_mask = platform::get_platform_state()->page_size - 1;

  cube_assert_debug(ptr != nullptr);
  cube_assert_debug((reinterpret_cast<usize>(ptr) & page_mask) == 0);
  cube_assert_debug((size & page_mask) == 0);
  void *block = VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE);
  cube_assert_debug(block != nullptr);
}

void decommit(void *ptr, usize size)
{
  usize page_mask = platform::get_platform_state()->page_size - 1;

  cube_assert_debug(ptr != nullptr);
  cube_assert_debug((reinterpret_cast<usize>(ptr) & page_mask) == 0);
  cube_assert_debug((size & page_mask) == 0);
  bool res = VirtualFree(ptr, size, MEM_DECOMMIT);
  cube_assert_debug(res);
}

// bool Window::create(Vec2i pos, Vec2i dem, Vec2i min_dem, Vec2i max_dem)
// {
//   if (win_callbacks.on_create) return on_create(callbacks.user_data);
//   return true;
// }

// void Window::destroy()
// {
// }

// void Window::set_callbacks(WindowCallbacks *win_callbacks)
// {
//   callbacks = *win_callbacks;
// }

// void Window::map()
// {
// }

// void Window::unmap()
// {
// }

// bool Window::pump_messages()
// {
// }

#endif // os_windows

}
