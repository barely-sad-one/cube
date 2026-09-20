#pragma once

namespace cube
{

void report_assertion_failure(const char *expr, const char *file, usize line, const char *message = nullptr);

}

#define cube_assert(expr)                                             \
  do                                                                  \
  {                                                                   \
    if (!(expr))                                                      \
      {                                                               \
        ::cube::report_assertion_failure(#expr, __FILE__, __LINE__);  \
        trap();                                                       \
      }                                                               \
  }                                                                   \
  while (0)
  
#define cube_assert_msg(expr, message)                                      \
  do                                                                        \
  {                                                                         \
    if (!(expr))                                                            \
    {                                                                       \
      ::cube::report_assertion_failure(#expr, __FILE__, __LINE__, message); \
      trap();                                                               \
    }                                                                       \
  }                                                                         \
  while (0)


#if defined(_DEBUG)
  #define cube_assert_debug(expr)                                       \
    do                                                                  \
    {                                                                   \
      if (!(expr))                                                      \
        {                                                               \
          ::cube::report_assertion_failure(#expr, __FILE__, __LINE__);  \
          trap();                                                       \
        }                                                               \
    }                                                                   \
    while (0)

  #define cube_assert_msg_debug(expr, message)                                \
    do                                                                        \
    {                                                                         \
      if (!(expr))                                                            \
      {                                                                       \
        ::cube::report_assertion_failure(#expr, __FILE__, __LINE__, message); \
        trap();                                                               \
      }                                                                       \
    }                                                                         \
    while (0)

#else
  #define cube_assert_debug(expr)                                       \
  #define cube_assert_msg_debug(expr, message)                                \

#endif
