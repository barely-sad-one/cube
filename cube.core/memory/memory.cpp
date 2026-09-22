namespace cube
{
  
static Memory g_memory_system;

void Memory::add(MemoryType type, usize size)
{
  usage[type] += size;
}

void Memory::sub(MemoryType type, usize size)
{
  usage[type] -= size;
}

Memory* get_memory_system()
{
  return &g_memory_system;
}

bool memory_initialize()
{
  g_memory_system.usage = {};
  g_memory_system.total_heap_usage = 0;

  return true;
}

void memory_destroy()
{
  // nothing to do here!
}

MemoryHeader* get_memory_header(void *ptr)
{
#if cube_memory_track
  u32 *offset_ptr = reinterpret_cast<u32*>(reinterpret_cast<usize>(ptr) - sizeof(u32));
  usize raw_addr = reinterpret_cast<usize>(ptr) - *offset_ptr;
  return reinterpret_cast<MemoryHeader*>(raw_addr);
#else
  return nullptr
#endif
}

void* reserve(usize size)
{
  return platform::reserve(size);
}

void release(void *ptr, usize size)
{
  platform::release(ptr, size);
}

void commit(void *ptr, usize size)
{
  platform::commit(ptr, size);
}

void decommit(void *ptr, usize size)
{
  platform::decommit(ptr, size);
}

}
