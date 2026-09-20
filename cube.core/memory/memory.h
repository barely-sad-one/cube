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
  MemoryType type;
  u32 offset;
};
static_assert(sizeof(MemoryHeader) == 16);

cube_api MemoryHeader* get_memory_header(void *ptr);

template <MemoryType type = MemoryType_Unknown, align_t alignment = align_default>
void* allocate(usize size)
{
  cube_assert_debug(size > 0);
#if cube_memory_track
  usize align = alignment;
  if (align < alignof(MemoryHeader)) align = alignof(MemoryHeader);

  usize total_size = size + sizeof(MemoryHeader) + align - 1;
  void *raw = platform::allocate<align>(total_size);
  if (!raw) return nullptr;

  usize base = reinterpret_cast<usize>(raw) + sizeof(MemoryHeader);
  usize aligned = utility::align_up(base, align);

  MemoryHeader *header = reinterpret_cast<MemoryHeader*>(aligned - sizeof(MemoryHeader));
  header->size = size;
  header->type = type;
  header->offset = static_cast<u32>(aligned) - reinterpret_cast<usize>(raw); 

  get_memory_system()->add(type, total_size);

  return reinterpret_cast<void*>(aligned);

#endif

  return platform::allocate<alignment>(size);
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

  MemoryHeader *header = get_memory_header(ptr);
  usize old_size = header->size + header->offset;

  i64 diff = header->size - old_size;
  if (diff > 0) get_memory_system()->add(header->type, static_cast<usize>(diff));
  else get_memory_system()->sub(header->type, static_cast<usize>(-diff));

#endif
  return platform::reallocate(ptr, new_size, alignment);
}

template <typename T, align_t alignment = align_default>
T* reallocate(T *ptr, usize new_count)
{
  return static_cast<T*>(reallocate<alignment>(static_cast<void*>(ptr), sizeof(T) * new_count));
}

void deallocate(void *ptr)
{
#if cube_memory_track
  MemoryHeader *header = get_memory_header(ptr);
  get_memory_system()->sub(header->type, header->size + header->offset);
#endif
  platform::deallocate(ptr);
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
