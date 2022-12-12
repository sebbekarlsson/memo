#define MEMO_ASSERT_PRINT false


#include <stdio.h>
#include <stdlib.h>
#include <memo_test/test.h>
#include <memo/memo.h>
#include <string.h>
#include <math.h>

void test_small() {
  Memo memo = {0};
  memo_init(&memo, (MemoConfig){ .item_size = sizeof(int), .page_capacity = 16 });
  MEMO_ASSERT(memo.initialized == true);


  int count = 32;


  for (int i = 0; i < count; i++) {

    int* my_int = memo_malloc(&memo);

    MEMO_ASSERT(my_int != 0);
    MEMO_ASSERT(*my_int == 0);
  }

  MEMO_ASSERT(memo.pages_length == 2);

  memo_clear(&memo);

  MEMO_ASSERT(memo.pages_length == 0);
}

typedef struct {
  char* name;
  int age;
} Person;

void person_destructor(Person* person) {
  MEMO_ASSERT(person->name != 0);

  free(person->name);
  person->name = 0;
  person->age = 0;
}

void test_custom_destructor(int count) {
  Memo memo = {0};
  memo_init(&memo, (MemoConfig){
    .item_size = sizeof(Person),
    .page_capacity = 16,
    .destructor = (MemoDestructorFunc)person_destructor
  });
  MEMO_ASSERT(memo.initialized == true);



  const char* names[] = {
  "john",
  "sarah",
  "david",
  "hannah"
};

  int nr_names = 4;

  for (int i = 0; i < count; i++) {
    Person* person = memo_malloc(&memo);
    MEMO_ASSERT(person != 0);

    person->name = strdup(names[i % nr_names]);

    MEMO_ASSERT(person->name != 0);
  }

  MEMO_ASSERT(memo.pages_length == count / memo.config.page_capacity);

  memo_clear(&memo);
}

void test_iter(int count) {
  Memo memo = {0};
  memo_init(&memo, (MemoConfig){
    .item_size = sizeof(Person),
    .page_capacity = 16,
    .destructor = (MemoDestructorFunc)person_destructor
  });
  MEMO_ASSERT(memo.initialized == true);



  const char* names[] = {
  "john",
  "sarah",
  "david",
  "hannah"
};

  int nr_names = 4;

  for (int i = 0; i < count; i++) {
    Person* person = memo_malloc(&memo);
    MEMO_ASSERT(person != 0);

    person->name = strdup(names[i % nr_names]);

    MEMO_ASSERT(person->name != 0);
  }

  MEMO_ASSERT(memo.pages_length == count / memo.config.page_capacity);


  MemoIterator it = {0};

  void* ptr = 0;

  int64_t nr_iterations = 0;
  while ((ptr = memo_iter(&memo, &it))) {
    MEMO_ASSERT(ptr != 0);

    Person* p = (Person*)ptr;

    const char* wanted_name = names[nr_iterations % nr_names];
    MEMO_ASSERT(p->name != 0);
    MEMO_ASSERT(strcmp(p->name, wanted_name) == 0);

    nr_iterations++;
  }

  MEMO_ASSERT(nr_iterations == count);


  memo_clear(&memo);
}

int main(int argc, char* argv[]){


  test_small();
  test_custom_destructor(64);
  test_custom_destructor(1024);
  test_iter(1024);

  printf("Finished.\n");
  return 0;
}
