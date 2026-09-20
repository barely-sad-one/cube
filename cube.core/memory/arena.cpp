namespace cube
{

ArenaBlock* create_arena_block(usize cap, usize cmt_size, usize start_base_pos)
{
  cube_assert_debug(cap      >= Arena::minimum_capacity);
  cube_assert_debug(cmt_size >= Arena::minimum_commit_size);

  void *ptr = reserve(cap);
  commit(ptr, cmt_size);
  ArenaBlock *block = reinterpret_cast<ArenaBlock*>(ptr);

  block->capacity = cap;
  block->commited = cmt_size;
  block->commit_size = cmt_size;
  block->used = arena_block_size;
  block->pos = start_base_pos + arena_block_size;
  block->prev = nullptr;
  
  return block;
}

void destroy_arena_block(ArenaBlock *block, usize capacity)
{
  cube_assert_debug(block != nullptr);
  release(reinterpret_cast<void*>(block), capacity);
}

bool Arena::create(usize cap, usize cmt_size, ArenaFlags f)
{
  cube_assert_debug(cap      >= minimum_capacity);
  cube_assert_debug(cmt_size >= minimum_commit_size);

  flags = f;
  blocks = create_arena_block(cap, cmt_size, 0);
  free_blocks = nullptr;

  return true;
}

void Arena::destroy()
{
  deallocate_free_blocks();
}

void Arena::deallocate_free_blocks()
{
#if defined(CUBE_ARENA_FREE_LIST)
  // TODO:
#endif
}

void Arena::allocate_block(usize cap, usize cmt_size)
{
  cube_assert_debug((flags & ArenaFlag_NoChain) != 0);
  ArenaBlock *block = create_arena_block(cap, cmt_size, blocks->pos + blocks->capacity);

  block->prev = blocks;
  blocks = block;
}

usize Arena::pos() { return blocks->pos; }

void Arena::pop_to(usize pos)
{
  (void) pos;
  // TODO: you wait bro
}

}
