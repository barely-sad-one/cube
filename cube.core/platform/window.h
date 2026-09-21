#pragma once

namespace cube
{

using window_create_fn      = bool(void *user_data);
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

struct WindowInternalState;

struct Window
{
  Vec2i   pos;
  Vec2i   dem;
  Vec2i   min_dem;
  Vec2i   max_dem;
  String8 title;
  WindowCallbacks callbacks;

  WindowInternalState *state;

  bool create(Arena *arena,
              StringView8 win_title,
              Vec2i p = {},
              Vec2i d = {},
              Vec2i min_d = {},
              Vec2i max_d = {});
  void destroy();

  void set_callbacks(WindowCallbacks *win_callbacks);

  bool map();
  bool unmap();

  bool pump_messages();
};


}
