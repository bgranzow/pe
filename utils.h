#ifndef PE_UTILS_H
#define PE_UTILS_H

namespace pe {

void print(const char* format, ...)
  __attribute__((format(printf,1,2)));
void fail(const char* format, ...)
  __attribute__((noreturn, format(printf,1,2)));
void failByAssert(const char* cond, const char* file, int line)
  __attribute__((noreturn));

}

#define ASSERT(c) ((c)?((void)0):pe::failByAssert(#c,__FILE__,__LINE__))
#define CALL(c) ASSERT((c)==0)


#endif
