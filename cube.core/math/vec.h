#pragma once

namespace cube
{

template <typename T>
struct Vec2
{
  union
  {
    T data[2];
    T x, y;
    T u, v;
    T width, height;
  };
};

using Vec2i  = Vec2<i32>;
using Vec2u  = Vec2<u32>;
using Vec2f  = Vec2<f32>;
using Vec2ii = Vec2<i64>;
using Vec2uu = Vec2<u64>;
using Vec2ff = Vec2<f64>;

template <typename T>
struct Vec3
{
  union
  {
    T data[3];
    T x, y, z;
  };
};

using Vec3i  = Vec3<i32>;
using Vec3u  = Vec3<u32>;
using Vec3f  = Vec3<f32>;
using Vec3ii = Vec3<i64>;
using Vec3uu = Vec3<u64>;
using Vec3ff = Vec3<f64>;

}
