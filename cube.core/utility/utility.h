#pragma once

namespace cube
{

using align_t = usize;

constexpr align_t align_default = 16;
constexpr align_t align_2 = 2;
constexpr align_t align_4 = 4;
constexpr align_t align_8 = 8;
constexpr align_t align_16 = 16;
constexpr align_t align_32 = 32;
constexpr align_t align_64 = 64;

constexpr usize operator""_kb(usize value) { return value * 1024; }
constexpr usize operator""_mb(usize value) { return value * 1024_kb; }
constexpr usize operator""_gb(usize value) { return value * 1024_mb; }

}

namespace cube::utility
{

constexpr bool is_pow2(usize value)
{
  return (value > 0) && (value & (value - 1)) == 0;
}

constexpr usize align_up(usize value, align_t alignment)
{
  cube_assert_debug(value > 0);
  cube_assert_debug(is_pow2(alignment));
  return (value + (alignment - 1)) & ~(alignment - 1);
}

constexpr bool is_aligned(usize value, align_t alignment)
{
  return alignment > 0 && (alignment & (alignment - 1)) == 0 && (value & (alignment - 1)) == 0;
}

template <typename F>
struct ExitScope
{
  F f;

  ExitScope(F f) : f(f) {}
  ~ExitScope() {f();}

  delete_copy_constructor(ExitScope);
  delete_move_constructor(ExitScope);
};

template <typename F>
struct ExitScopeHelper
{
  ExitScope<F> operator+ (F f) { return ExitScope<F>(f); }
};

}

#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)

#define defer const auto& concat(_defer_v_, __COUNTER__) = ExitScopeHelper() + [&]()
