#ifndef MEMO_PAGE_H
#define MEMO_PAGE_H
#include <stdint.h>
#include <stdbool.h>


typedef struct {
  uint8_t* data;
  int64_t cursor;
  int64_t capacity;
  int64_t item_size;
  bool initialized;
} MemoPage;

int memo_page_init(
  MemoPage* page,
  int64_t capacity,
  int64_t item_size
);


void* memo_page_malloc(MemoPage* page);

int memo_page_clear(MemoPage* page);

bool memo_page_is_full(MemoPage* page);

#endif
