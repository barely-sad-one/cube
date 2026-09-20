#pragma once

//
// Compiler Options (/D) -
//   CUBE_ARENA_FREE_LIST - Enable Free List in Arena Allocator
//

namespace cube
{

enum ArenaFlags : u8
{
  ArenaFlag_Chain     = 1 << 0,
  ArenaFlag_NoChain   = 1 << 1,

  ArenaFlag_LargePage = 1 << 2,
};
  
struct ArenaBlock
{
  usize capacity;
  usize commited;
  usize commit_size; // commit size must be
                     // equal or greater than page size
                     // and also aligned with page size and capacity
  usize used;
  usize pos;

  ArenaBlock *prev;
};

ArenaBlock* create_arena_block(usize cap, usize cmt_size, usize start_base_pos);
void destroy_arena_block(ArenaBlock *block, usize capacity);

static_assert(sizeof(ArenaBlock) <= 512);
constexpr usize arena_block_size = 512;

class ArenaTemp
{
};

struct cube_api Arena
{
  ArenaFlags flags;
  ArenaBlock *blocks;
  ArenaBlock *free_blocks;

  static constexpr usize default_capacity    = 64_mb;
  static constexpr usize minimum_capacity    = 64_mb;
  static constexpr usize minimum_commit_size = 2_mb;

  bool create(usize cap = default_capacity, usize cmt_size = minimum_commit_size, ArenaFlags f = ArenaFlag_Chain);
  void destroy();

  void allocate_block(usize cap, usize cmt_size);
  void deallocate_free_blocks();

  template <align_t alignment = align_default>
  void* push(usize size)
  {
    usize ptr = reinterpret_cast<usize>(blocks) + blocks->pos;
    usize aligned = utility::align_up(ptr, alignment);
    usize offset = aligned - ptr;
    usize total_size = offset + size;
    usize used;
    if (total_size > blocks->capacity - blocks->used)
    {
      allocate_block(blocks->capacity, blocks->commit_size);
      ptr = reinterpret_cast<usize>(blocks) + blocks->pos;
      aligned = utility::align_up(ptr, alignment);
      offset = aligned - ptr;
      total_size = offset + size;
    }

    used = blocks->used + total_size;
    blocks->used = used;
    blocks->pos += total_size;
    return reinterpret_cast<void*>(aligned);
  }

  template <typename T, align_t alignment = align_default>
  T* push(usize size)
  {
    return static_cast<T*>(push<alignment>(size));
  }

  usize pos();
  void pop_to(usize pos);
};

}
