#pragma once

//
// Compiler Options (/D) -
//   CUBE_MEMORY_TRACK                    - Enable Memory Tracking on the basis of MemoryType
//

#define cube_memory_track      0

#if defined(CUBE_MEMORY_TRACK)
  #undef cube_memory_track
  #define cube_memory_track  1
#endif

namespace cube
{

enum MemoryType : u32
{
  MemoryType_Unknown,
  MemoryType_String,

  MemoryType_ArenaReserved,
  MemoryType_ArenaCommitted,

  _MemoryType_Count,
};

struct Memory
{
  Array<usize, _MemoryType_Count> usage;
  usize total_heap_usage;

  void add(MemoryType type, usize size);
  void sub(MemoryType type, usize size);
};

bool memory_initialize();
void memory_destroy();

cube_api Memory* get_memory_system();

struct MemoryHeader
{
  usize size;
  usize total_size;
  MemoryType type;
};
static_assert(sizeof(MemoryHeader) == 24); // 8 + 8 + 4 + 4(implicit struct padding) = 24 bytes

cube_api MemoryHeader* get_memory_header(void *ptr);

template <MemoryType type = MemoryType_Unknown, align_t alignment = align_default>
void* allocate(usize size)
{
  cube_assert_debug(size > 0);
#if cube_memory_track
  usize align = alignment;
  if (align < alignof(MemoryHeader)) align = alignof(MemoryHeader);

  usize total_size = sizeof(MemoryHeader) + sizeof(u32) + size + align - 1;
  void *raw = platform::allocate(total_size, alignment);
  if (!raw) return nullptr;

  usize raw_addr = reinterpret_cast<usize>(raw);

  MemoryHeader *header = reinterpret_cast<MemoryHeader*>(raw);
  header->size = size;
  header->total_size = total_size;
  header->type = type;

  usize user_addr = utility::align_up(raw_addr + sizeof(MemoryHeader) + sizeof(u32), align);

  u32 *offset_ptr = reinterpret_cast<u32*>(user_addr - sizeof(u32));
  *offset_ptr = static_cast<u32>(user_addr - raw_addr);

  get_memory_system()->add(type, total_size);

  return reinterpret_cast<void*>(user_addr);
#else

  return platform::allocate(size, alignment);
#endif
}

template <typename T, MemoryType type = MemoryType_Unknown, align_t alignment = align_default>
T* allocate(usize count)
{
  return static_cast<T*>(allocate<type, alignment>(sizeof(T) * count));
}

template <align_t alignment = align_default>
void* reallocate(void *ptr, usize new_size)
{
  cube_assert_debug(new_size > 0);
#if cube_memory_track
  if (!ptr) return allocate<MemoryType_Unknown, alignment>(new_size);

  u32 *offset_ptr = reinterpret_cast<u32*>(reinterpret_cast<usize>(ptr) - sizeof(u32));
  usize old_raw_addr = reinterpret_cast<usize>(ptr) - *offset_ptr;
  MemoryHeader *old_header = reinterpret_cast<MemoryHeader*>(old_raw_addr);

  MemoryType tracking_type = old_header->type;
  usize old_total_size = old_header->total_size;

  usize align = alignment;
  if (align < alignof(MemoryHeader)) align = alignof(MemoryHeader);
  usize new_total_size = sizeof(MemoryHeader) + sizeof(u32) + new_size + align - 1;

  void *new_raw = platform::reallocate(reinterpret_cast<void*>(old_raw_addr), new_total_size, align);
  if (!new_raw) return nullptr;

  usize new_raw_addr = reinterpret_cast<usize>(new_raw);

  MemoryHeader *new_header = reinterpret_cast<MemoryHeader*>(new_raw);
  new_header->size = new_size;
  new_header->total_size = new_total_size;
  new_header->type = tracking_type;

  usize new_user_addr = utility::align_up(new_raw_addr + sizeof(MemoryHeader) + sizeof(u32), align);

  u32 *new_offset_ptr = reinterpret_cast<u32*>(new_user_addr - sizeof(u32));
  *new_offset_ptr = static_cast<u32>(new_user_addr - new_raw_addr);

  get_memory_system()->sub(tracking_type, old_total_size);
  get_memory_system()->add(tracking_type, new_total_size);

  return reinterpret_cast<void*>(new_user_addr);

#else
  return platform::reallocate(ptr, new_size, alignment);

#endif
}

template <typename T, align_t alignment = align_default>
T* reallocate(T *ptr, usize new_count)
{
  return static_cast<T*>(reallocate<alignment>(static_cast<void*>(ptr), sizeof(T) * new_count));
}

void deallocate(void *ptr)
{
  void *block = ptr;
#if cube_memory_track
  u32 *offset_ptr = reinterpret_cast<u32*>(reinterpret_cast<usize>(ptr) - sizeof(u32));
  usize raw_addr = reinterpret_cast<usize>(ptr) - *offset_ptr;
  MemoryHeader *header = reinterpret_cast<MemoryHeader*>(raw_addr);
  get_memory_system()->sub(header->type, header->total_size);

  block = reinterpret_cast<void*>(raw_addr);
#endif
  platform::deallocate(block);
}

template <typename T>
void deallocate(T *ptr)
{
  deallocate(static_cast<void*>(ptr));
}

cube_api void* reserve(usize size);
cube_api void release(void *ptr, usize size);
cube_api void commit(void *ptr, usize size);
cube_api void decommit(void *ptr, usize size);

}
