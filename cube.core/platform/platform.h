#pragma once

namespace cube::platform
{

struct PlatformState
{
  i32 logical_processors;
  usize allocation_granularity;
  usize page_size;
  usize large_page_size;
};

bool platform_initialize();
void platform_destroy();

cube_api PlatformState* get_platform_state();
cube_api align_t align_page_size();

cube_api void* allocate(usize size, align_t alignment = align_default);
cube_api void* reallocate(void *ptr, usize new_size, align_t alignment = align_default);
cube_api void deallocate(void *ptr);

template <typename T>
T* allocate(usize count, align_t alignment = align_default)
{
  return static_cast<T*>(platform::allocate(sizeof(T) * count, alignment));
}

template <typename T>
T* reallocate(T* ptr, usize count)
{
  return static_cast<T*>(platform::reallocate(static_cast<void*>(ptr), sizeof(T) * count));
}

template <typename T>
void deallocate(T *ptr)
{
  platform::deallocate(static_cast<void*>(ptr));
}

cube_api void* reserve(usize size);
cube_api void release(void *ptr, usize size);
cube_api void commit(void *ptr, usize size);
cube_api void decommit(void *ptr, usize size);

cube_api void* memory_copy(void *dest, const void *src, usize size);
cube_api void* memory_move(void *dest, void *src, usize size);
cube_api void* memory_set(void *dest, i32 value, usize size);
cube_api void* memory_zero(void *dest, usize size);
cube_api i32   memory_compare(const void *a, const void *b, usize size);

template <typename T>
T* memory_copy(T *dest, const T *src, usize count)
{
  return static_cast<T*>(memory_copy(static_cast<void*>(dest), static_cast<const void*>(src), sizeof(T) * count));
}

template <typename T>
T* memory_move(T *dest, T *src, usize count)
{
  return static_cast<T*>(memory_move(static_cast<void*>(dest), static_cast<void*>(src), sizeof(T) * count));
}

template <typename T>
T* memory_set(T *dest, i32 value, usize count)
{
  return static_cast<T*>(memory_set(static_cast<void*>(dest), value, sizeof(T) * count));
}

template <typename T>
T* memory_zero(T *dest, usize count)
{
  return static_cast<T*>(memory_zero(static_cast<void*>(dest), sizeof(T) * count));
}

template <typename T>
i32 memory_compare(const T *a, const T *b, usize count)
{
  return memory_compare(static_cast<const void*>(a), static_cast<const void*>(b), sizeof(T) * count);
}

}
