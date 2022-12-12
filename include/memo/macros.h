#ifndef MEMO_MACROS_H
#define MEMO_MACROS_H
#include <stdio.h>


#ifndef NEW
#define NEW(T) ((T *)(calloc(1, sizeof(T))))
#endif

#ifndef OR
#define OR(a, b) ((a) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) ((a) < b ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif


#define MEMO_ALIGN_UP(x, n) (((x) + (n) - 1) & ~((n) - 1))

#define MEMO_IS_POWER_OF_2(x) ((x != 0) && ((x & (x - 1)) == 0))


#define MEMO_WARNING(...)                                                      \
  {                                                                            \
    printf("(MEMO)(Warning)(%s): \n", __func__);   \
    fprintf(__VA_ARGS__);                                                      \
  }
#define MEMO_WARNING_RETURN(ret, ...)                                          \
  {                                                                            \
    printf("\n****\n");                                                        \
    printf("(MEMO)(Warning)(%s): \n", __func__);   \
    fprintf(__VA_ARGS__);                                                      \
    printf("\n****\n");                                                        \
    return ret;                                                                \
  }

#endif
