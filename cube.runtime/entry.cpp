#include <cube.core/core_inc.h>
#include <cube.core/core_inc.cpp>

namespace cube
{

i32 entry()
{

  platform::platform_initialize();

  i32 *ptr = platform::allocate<i32>(8);
  ptr = platform::reallocate<i32>(ptr, 16);

  platform::deallocate(ptr);

  platform::platform_destroy();

  // cube_assert_msg_debug(0, "This is a assertion message.");

  Arena a;
  a.create(64_mb, 2_mb, ArenaFlag_Chain);

  a.destroy();

  return 0;
}

}
