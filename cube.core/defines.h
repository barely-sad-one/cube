#pragma once

#include <stddef.h>

namespace cube
{

using i8  = signed char;
using i16 = signed short;
using i32 = signed int;
using i64 = signed long long;
static_assert(sizeof(i8)  == 1);
static_assert(sizeof(i16) == 2);
static_assert(sizeof(i32) == 4);
static_assert(sizeof(i64) == 8);

using u8  = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;
static_assert(sizeof(u8)  == 1);
static_assert(sizeof(u16) == 2);
static_assert(sizeof(u32) == 4);
static_assert(sizeof(u64) == 8);

using f32 = float;
using f64 = double;
static_assert(sizeof(f32) == 4);
static_assert(sizeof(f64) == 8);

using usize = size_t;
using isize = ptrdiff_t;
static_assert(sizeof(usize) == 8);
static_assert(sizeof(isize) == 8);

}

// compiler detection
#define compiler_msvc  0
#define compiler_clang 0
#define compiler_gcc   0

#if defined(_MSC_VER)
  #undef compiler_msvc
  #define compiler_msvc 1

#elif defined(__clang__)
  #undef compiler_clang
  #define compiler_clang 1

#elif defined(__GNUC__) || defined(__GNUG__)
  #undef compiler_gcc
  #define compiler_gcc 1

#else
  #error "Unknown Compiler"

#endif // compiler detection

// os detection
#define os_windows 0
#define os_linux 0

#if defined(_WIN64)
  #undef os_windows
  #define os_windows 1

#elif defined(__linux__)
  #undef os_linux
  #define os_linux 1

#else
  #error "Unknown OS"

#endif // os detection

// arch detection
#define arch_arm 0
#define arch_x86 0

#if defined(__x86_64__) || defined(_M_X64)
  #undef arch_x86
  #define arch_x86 1

#elif defined(__aarch64__) || defined(_M_ARM64)
  #undef arch_arm
  #define arch_arm 1

#else
  #error "Unknown Arch"

#endif // arch detection

// macros
#if compiler_msvc
  #if defined (CUBE_EXPORT)
    #define cube_api __declspec(dllexport)
  #else 
    #define cube_api __declspec(dllimport)
  #endif

  #if defined (_DEBUG)
    #define trap() do { __debugbreak(); } while (0)
    #define unreachable() trap()

  #else
    #define trap() do { __fastfail(FAST_FAIL_FATAL_APP_EXIT); } while (0)
    #define unreachable() __assume(false)
  #endif

#else
  #error "Macros are only defined of MSVC compiler currenly"

#endif // macros


// delete copy constructor
#define delete_copy_constructor(class_name) \
  class_name(const class_name&) = delete; \
  class_name& operator=(const class_name&) = delete

// delete move constructor
#define delete_move_constructor(class_name) \
  class_name(class_name&&) = delete; \
  class_name& operator=(class_name&&) = delete
