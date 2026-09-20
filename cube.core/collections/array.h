#pragma once

namespace cube
{

template <typename T, usize N>
struct Array
{
  using type = T;
  using pointer_type = T*;

  T data[N];

  constexpr T& operator[](usize idx) const
  {
    cube_assert_debug(idx < N);
    return data[idx];
  }

  constexpr T& operator[](usize idx)
  {
    cube_assert_debug(idx < N);
    return data[idx];
  }

  constexpr usize size() const { return N; }

  constexpr T* begin() { return &data[0]; }
  constexpr T* end() { return &data[N]; }

  constexpr T* begin() const { return &data[0]; }
  constexpr T* end() const { return &data[N]; }
};

}
