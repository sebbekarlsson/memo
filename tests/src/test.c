#include <assert.h>
#include <memo_test/test.h>
#include <stdio.h>
#include <stdlib.h>

int memo_assert(bool condition, const char *message, const char *funcname,
                bool log) {

  if (log) {
    printf("Test: %s\n", funcname);
    printf("%s\n", message);
    if (!condition) {
      fprintf(stderr, "Condition failed: %s\n", message ? message : "?");
      exit(0);
    }
    printf("OK.\n");
  } else {
    assert(condition);
  }
  return 1;
}
