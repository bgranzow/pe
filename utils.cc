#include "utils.h"

#include <PCU.h>
#include <cstdlib>
#include <cstdarg>

namespace pe {

void print(const char* format, ...)
{
  if (PCU_Comm_Self())
    return void();
  printf("pe: ");
  va_list ap;
  va_start(ap, format);
  vfprintf(stdout, format, ap);
  va_end(ap);
  printf("\n");
}

void fail(const char* format, ...)
{
  va_list ap;
  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);
  printf("\n");
  abort();
}

void failByAssert(const char* cond, const char* file, int line)
{
  fail("assertion failed: '%s' %s:%i\n", cond, file, line);
}

}
