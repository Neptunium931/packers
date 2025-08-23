#include "util.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

// NOLINTBEGIN(cppcoreguidelines-pro-type-vararg,
// cppcoreguidelines-pro-bounds-array-to-pointer-decay)
[[noreturn]] void
fatal(const char *msg, ...)
{
  va_list args;
  va_start(args, msg);
  vfprintf(stderr, msg, args);
  va_end(args);
  exit(1); // NOLINT(concurrency-mt-unsafe)
}
// NOLINTEND(cppcoreguidelines-pro-type-vararg,
// cppcoreguidelines-pro-bounds-array-to-pointer-decay)
