#include <memo/macros.h>
#include <memo/memo.h>
#include <stdlib.h>
#include <string.h>

int memo_init(Memo *memo, MemoConfig cfg) {
  if (!memo)
    return 0;
  if (memo->initialized)
    return 1;
  if (cfg.item_size <= 0)
    MEMO_WARNING_RETURN(0, stderr, "config.item_size <= 0");
  if (cfg.page_capacity <= 0)
    MEMO_WARNING_RETURN(0, stderr, "config.page_capacity <= 0");

  if (pthread_mutex_init(&memo->lock, 0)) {
    MEMO_WARNING_RETURN(0, stderr, "Failed to initialize mutex.\n");
  }

  memo->initialized = true;

  memo->config = cfg;
  memo->pages = 0;
  memo->pages_length = 0;

  return 1;
}

static MemoPage *memo_get_last_page(Memo *memo) {
  return &memo->pages[MAX(0, memo->pages_length - 1)];
}

static int memo_grow(Memo *memo) {
  memo->pages = (MemoPage *)realloc(memo->pages, (memo->pages_length + 1) *
                                                     sizeof(MemoPage));
  memo->pages_length++;

  MemoPage *page = memo_get_last_page(memo);
  memset(&page[0], 0, sizeof(MemoPage));
  *page = (MemoPage){0};

  return memo->pages != 0;
}

static bool memo_has_pages(Memo *memo) {
  return memo->pages != 0 && memo->pages_length > 0;
}

static MemoPage *memo_get_avail_page(Memo *memo) {
  if (!memo_has_pages(memo)) {
    if (!memo_grow(memo))
      MEMO_WARNING_RETURN(0, stderr, "failed to grow pages.\n");
  }

  for (int64_t i = 0; i < memo->pages_length; i++) {
    MemoPage *page = &memo->pages[i];
    if (!page->initialized) {
      memo_page_init(page, memo->config.page_capacity, memo->config.item_size);
      return page;
    }
    if (!memo_page_is_full(page)) {
      return page;
    }
  }

  if (!memo_grow(memo))
    MEMO_WARNING_RETURN(0, stderr, "failed to grow pages.\n");

  MemoPage *last_page = memo_get_last_page(memo);
  if (!last_page)
    return 0;
  if (last_page->initialized)
    MEMO_WARNING_RETURN(0, stderr, "expected page to not be initialized.\n");
  memo_page_init(last_page, memo->config.page_capacity, memo->config.item_size);

  return last_page;
}

void *memo_malloc(Memo *memo) {
  if (!memo)
    return 0;

  pthread_mutex_lock(&memo->lock);

  if (!memo->initialized) {
    pthread_mutex_unlock(&memo->lock);
    MEMO_WARNING_RETURN(0, stderr, "memo not initialized.\n");
  }

  MemoPage *page = memo_get_avail_page(memo);

  if (!page) {
    pthread_mutex_unlock(&memo->lock);
    MEMO_WARNING_RETURN(0, stderr, "Failed to fetch page.\n");
  }

  void *ptr = memo_page_malloc(page);

  pthread_mutex_unlock(&memo->lock);

  return ptr;
}

int memo_clear(Memo *memo) {
  if (!memo)
    return 0;
  if (!memo->initialized)
    MEMO_WARNING_RETURN(0, stderr, "memo not initialized.\n");

  if (memo->pages != 0) {
    for (int64_t i = 0; i < memo->pages_length; i++) {
      MemoPage *page = &memo->pages[i];

      if (memo->config.destructor) {
        for (int64_t i = 0; i < page->cursor; i += page->item_size) {
          void *ptr = &page->data[i];
          memo->config.destructor(ptr);
        }
      }
      memo_page_clear(page);
    }

    free(memo->pages);
  }

  memo->pages = 0;
  memo->pages_length = 0;

  return 1;
}

void *memo_iter(Memo *memo, MemoIterator *it) {
  if (!memo || !it)
    return 0;

  pthread_mutex_lock(&memo->lock);

  if (!memo->initialized) {
    pthread_mutex_unlock(&memo->lock);
    MEMO_WARNING_RETURN(0, stderr, "memo not initialized.\n");
  }
  if (memo->pages == 0 || memo->pages_length <= 0) {
    pthread_mutex_unlock(&memo->lock);
    return 0;
  }
  if (it->current_page >= memo->pages_length) {
    pthread_mutex_unlock(&memo->lock);
    return 0;
  }

  MemoPage *page = &memo->pages[it->current_page];

  if (it->page_cursor >= page->cursor) {
    if ((it->current_page + 1) >= memo->pages_length) {
      pthread_mutex_unlock(&memo->lock);
      return 0;
    }
    page = &memo->pages[it->current_page++];
    it->page_cursor = 0;
  }

  void *ptr = &page->data[it->page_cursor];
  it->page_cursor += memo->config.item_size;

  pthread_mutex_unlock(&memo->lock);

  return ptr;
}
