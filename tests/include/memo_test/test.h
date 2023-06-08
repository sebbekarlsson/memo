#ifndef MEMO_TEST_H
#define MEMO_TEST_H
#include <stdbool.h>


//#ifdef MEMO_ASSERT_PRINT
//#define MEMO_ASSERT(expr) memo_assert((expr), #expr, __func__, MEMO_ASSERT_PRINT)
//#else
//#define MEMO_ASSERT(expr) memo_assert((expr), #expr, __func__, false)
//#endif


#define MEMO_ASSERT(expr)\
  { if (!(expr)) { fprintf(stderr, "[FAIL](%s): ASSERT(%s)\n", __func__, #expr); } }

//int memo_assert(bool condition, const char* message, const char* funcname, bool log);
#endif
