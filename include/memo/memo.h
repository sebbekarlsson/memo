#ifndef MEMO_H
#define MEMO_H
#include <memo/page.h>
#include <pthread.h>

typedef void (*MemoDestructorFunc)(void* ptr);

typedef struct {
  int64_t item_size;
  int64_t page_capacity;
  MemoDestructorFunc destructor;
} MemoConfig;

typedef struct {
  MemoConfig config;
  MemoPage* pages;
  int64_t pages_length;
  int64_t count;

  pthread_mutex_t lock;

  bool initialized;
} Memo;

typedef struct {
  Memo* memo;
  int64_t current_page;
  int64_t page_cursor;
} MemoIterator;

int memo_init(Memo* memo, MemoConfig cfg);

void* memo_malloc(Memo* memo);

void* memo_malloc_n(Memo* memo, int64_t count);

int memo_clear(Memo* memo);

void* memo_iter(Memo* memo, MemoIterator* it);

#endif
