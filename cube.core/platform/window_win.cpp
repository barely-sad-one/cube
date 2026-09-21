namespace cube
{

struct WindowInternalState
{
  HWND handle;
};

inline constexpr cube::StringView16 window_class_name = L"CubeEngine";
static void _register_window_class();

static LRESULT CALLBACK _window_callback_proc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam);

bool Window::create(Arena *arena, StringView8 win_title, Vec2i p, Vec2i d, Vec2i min_d, Vec2i max_d)
{
  state = arena->push<WindowInternalState>(1);

  _register_window_class();
  pos = p;
  dem = d;
  min_dem = min_d;
  max_dem = max_d;

  title = create_string(arena, win_title);
  String16H _title = str16H_from_8(win_title);
  defer { _title.destroy(); };

  u32 ex_style = WS_EX_ACCEPTFILES;
  u32 style = WS_OVERLAPPEDWINDOW;
  state->handle = CreateWindowExW(ex_style,
                                  window_class_name.data,
                                  _title.data,
                                  style,
                                  pos.x,
                                  pos.y,
                                  dem.width,
                                  dem.height,
                                  nullptr,
                                  nullptr,
                                  GetModuleHandle(nullptr),
                                  nullptr);
  if (state->handle == nullptr) return false;

  if (callbacks.on_create) return callbacks.on_create(callbacks.user_data);
  return true;
}

void Window::destroy()
{
  // FIXME: error handling
  bool res = DestroyWindow(state->handle);
  state->handle = nullptr;

}

bool Window::map()
{
  return ShowWindowAsync(state->handle, SW_SHOW);
}

bool Window::unmap()
{
  return ShowWindowAsync(state->handle, SW_HIDE);
}

bool Window::pump_messages()
{
  bool quit = false;

  MSG msg;
  while (PeekMessageW(&msg, nullptr, 0, 0, PM_NOREMOVE))
  {
    switch (msg.message)
    {
      case WM_QUIT:
        {
          quit = true;
        } break;
      default:
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
  }

  return !quit;
}

void _register_window_class()
{
  static bool registered = false;
  if (registered) return;

  WNDCLASSEXW wc = {};
  wc.cbSize        = sizeof(wc);
  wc.style         = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc   = _window_callback_proc;
  wc.hInstance     = GetModuleHandle(nullptr);
  wc.hIcon         = nullptr;
  wc.hCursor       = LoadCursorW(nullptr, reinterpret_cast<LPCWSTR>(IDC_ARROW));
  wc.hbrBackground = nullptr;
  wc.lpszClassName = window_class_name.data;
  wc.hIconSm       = nullptr;

  ATOM result = RegisterClassExW(&wc);
  cube_assert(result != 0);

  registered = true;
}

LRESULT CALLBACK _window_callback_proc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
{
  switch (message)
  {
    case WM_DESTROY:
      {
        PostQuitMessage(0);
        return 0;
      } break;
  default:
    return DefWindowProcW(handle, message, wparam, lparam);
  }
}

}
