#pragma once

namespace cube::platform
{

using window_create_fn      = void(void *user_data);
using window_quit_fn        = void(void *user_data);
using window_key_fn         = void(void *user_data);
using window_button_fn      = void(void *user_data);
using window_mouse_move_fn  = void(void *user_data);
using window_mouse_scoll_fn = void(void *user_data);

struct WindowCallbacks
{
  window_create_fn *on_create;
  void *user_data;
};

struct Window
{
  Vec2i pos;
  Vec2i dem;
  Vec2i min_dem;
  Vec2i max_dem;
  bool  mapped;
  WindowCallbacks callbacks;

  bool create(Vec2i pos = {}, Vec2i dem = {}, Vec2i min_dem = {}, Vec2i max_dem = {});
  void destroy();

  void set_callbacks(WindowCallbacks *win_callbacks);

  void map();
  void unmap();

  bool pump_messages();
};


}
