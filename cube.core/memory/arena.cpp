namespace cube
{

ArenaBlock* create_arena_block(usize cap, usize cmt_size, usize start_base_pos)
{
  cube_assert_debug(cap      >= Arena::minimum_capacity);
  cube_assert_debug(cmt_size >= Arena::minimum_commit_size);

  void *ptr = reserve(cap);
  commit(ptr, cmt_size);
  ArenaBlock *block = reinterpret_cast<ArenaBlock*>(ptr);

  block->capacity    = cap;
  block->commited    = cmt_size;
  block->commit_size = cmt_size;
  block->used        = arena_block_size;
  block->base_pos    = start_base_pos;
  block->prev        = nullptr;
  
  return block;
}

void destroy_arena_block(ArenaBlock *block)
{
  while (block != nullptr)
  {
    ArenaBlock *prev = block->prev;
    release(block, block->capacity);
    block = prev;
  }
}

ArenaTemp::ArenaTemp(Arena *arena)
  : arena(arena), pos(arena->pos()) {}

ArenaTemp::~ArenaTemp()
{
  arena->pop_to(pos);
  arena = nullptr;
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
  destroy_arena_block(blocks);
  blocks = nullptr;
}

void Arena::deallocate_free_blocks()
{
#if defined(CUBE_ARENA_FREE_LIST)
  destroy_arena_block(free_blocks);
  free_blocks = nullptr;
#endif
}

void Arena::allocate_block(usize cap, usize cmt_size)
{
  cube_assert_debug((flags & ArenaFlag_NoChain) == 0);
  cube_assert_debug(cap >= minimum_capacity);
  
#if defined(CUBE_ARENA_FREE_LIST)
  ArenaBlock **node = &free_blocks;
  while (*node != nullptr)
  {
    if ((*node)->capacity >= cap)
    {
      ArenaBlock *block = *node;
      *node = block->prev;
 
      block->used     = arena_block_size;
      block->base_pos = blocks->base_pos + blocks->capacity;
      block->prev     = blocks;
      blocks = block;
      return;
    }
    node = &(*node)->prev;
  }
#endif

  ArenaBlock *block = create_arena_block(cap, cmt_size, blocks->base_pos + blocks->capacity);

  block->prev = blocks;
  blocks = block;
}

void Arena::grow_commit(usize size)
{
  usize needed = blocks->used + size;
 
  if (needed > blocks->commited)
  {
    usize deficit = needed - blocks->commited;
    usize to_commit = utility::align_up(deficit, blocks->commit_size);
 
    void *commit_ptr = reinterpret_cast<void*>(reinterpret_cast<usize>(blocks) + blocks->commited);
 
    commit(commit_ptr, to_commit);
    blocks->commited += to_commit;
  }
}

usize Arena::pos() const { return blocks->base_pos + blocks->used; }

void Arena::pop_to(usize pos)
{
  cube_assert_debug(pos <= this->pos());
 
  while (blocks->prev != nullptr && blocks->base_pos > pos)
  {
    ArenaBlock *stale = blocks;
    blocks = blocks->prev;
 
#if defined(CUBE_ARENA_FREE_LIST)
    stale->prev = free_blocks;
    free_blocks = stale;
#else
    release(stale, stale->capacity);
#endif
  }
 
  cube_assert_debug(blocks->base_pos <= pos);
 
  usize local = pos > blocks->base_pos ? pos - blocks->base_pos : 0;
  local = local < arena_block_size ? arena_block_size : local;
  cube_assert_debug(local <= blocks->capacity);
 
  blocks->used = local;

  usize target_commited = utility::align_up(local, blocks->commit_size);
  target_commited = target_commited < blocks->commit_size ? blocks->commit_size : target_commited;
 
  if (target_commited < blocks->commited)
  {
    void *decommit_ptr = reinterpret_cast<void*>(reinterpret_cast<usize>(blocks) + target_commited);
    decommit(decommit_ptr, blocks->commited - target_commited);
    blocks->commited = target_commited;
  }
}

}
