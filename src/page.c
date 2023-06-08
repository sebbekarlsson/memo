#include <memo/page.h>
#include <memo/macros.h>
#include <stdlib.h>


int memo_page_init(
  MemoPage* page,
  int64_t capacity,
  int64_t item_size
) {
  if (!page) return 0;
  if (page->initialized) return 1;
  if (capacity <= 0) MEMO_WARNING_RETURN(0, stderr, "capacity <= 0.\n");
  if (item_size <= 0) MEMO_WARNING_RETURN(0, stderr, "item_size <= 0.\n");
  page->initialized = true;


  page->capacity = capacity * item_size;
  page->item_size = item_size;
  page->cursor = 0;
  page->data = calloc(capacity, item_size);

  return page->data != 0;
}


void* memo_page_malloc(MemoPage* page) {
  if (!page) return 0;
  if (!page->initialized) MEMO_WARNING_RETURN(0, stderr, "page not initialized.\n");
  if (page->data == 0) MEMO_WARNING_RETURN(0, stderr, "page->data == 0.\n");
  if (page->cursor >= page->capacity) return 0;
  if (page->item_size <= 0) MEMO_WARNING_RETURN(0, stderr, "page->item_size <= 0.\n");


  uint8_t* bytes = &page->data[page->cursor];
  page->cursor += page->item_size;


  return bytes;
}

void* memo_page_malloc_n(MemoPage* page, int64_t count) {
  if (!page) return 0;
  if (!page->initialized) MEMO_WARNING_RETURN(0, stderr, "page not initialized.\n");
  if (page->data == 0) MEMO_WARNING_RETURN(0, stderr, "page->data == 0.\n");
  if (page->cursor >= page->capacity) return 0;
  if (page->item_size <= 0) MEMO_WARNING_RETURN(0, stderr, "page->item_size <= 0.\n");
  if (count <= 0) MEMO_WARNING_RETURN(0, stderr, "count <= 0.\n");


  uint8_t* bytes = &page->data[page->cursor];
  page->cursor += page->item_size * count;


  return bytes;
}

int memo_page_clear(MemoPage* page) {
  if (!page) return 0;
  if (!page->initialized) MEMO_WARNING_RETURN(0, stderr, "page not initialized.\n");

  if (page->data != 0) {
    free(page->data);
  }

  page->data = 0;
  page->cursor = 0;

  return 1;
}

bool memo_page_is_full(MemoPage* page) {
  if (!page) return true;
  if (!page->initialized) return false;
  return page->cursor >= page->capacity;
}
