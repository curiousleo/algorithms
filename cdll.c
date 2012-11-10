/*
 * cdll.c
 *
 * A simplistic circular doubly-linked list implementation.
 *
 * Leonhard Markert, 2012
 * Licensed under Creative Commons Attribution (cc-by).
 *
 */

#include <assert.h>   /* assert */
#include <stdlib.h>   /* malloc, free */

#include "cdll.h"

static const int CDLL_SIZE = sizeof(cdll);

cdll *cdll_init(void) {
  cdll *l = (cdll *) malloc(CDLL_SIZE);
  CDLL_SETEMPTY(l);
  return l;
}

/* void cdll_insert(cdll *l, item i); */
void cdll_insert(cdll *l, int i) {
  if (CDLL_ISEMPTY(l)) {
    l->item = i;
    l->next = l->prev = l;
  } else {
    cdll *newl = cdll_init();
    newl->item = i;

    l->prev->next = newl;
    newl->prev = l->prev;
    l->prev = newl;
    newl->next = l;
  }
}

/* void cdll_delete(cdll *l, item i); */
cdll *cdll_delete(cdll *l, int i) {
  cdll *lnext;
#ifndef NDEBUG
  cdll *ltmp = l;
#endif /* DEBUG */
  assert(!CDLL_ISEMPTY(l));
  if (CDLL_ISSINGLT(l)) {
    CDLL_SETEMPTY(l);
    return l;
  }
  while (l->item != i) {
    /* linear search for item */
    l = l->next;
    assert(l != ltmp);
  }
  lnext = l->next;
  cdll_free(l);
  return lnext;
}

void cdll_merge(cdll *l, cdll *l2) {
  cdll *l2copy;
  /* three cases: *l2* is empty / *l* is empty / neither is empty */
  if (CDLL_ISEMPTY(l2)) {
    /* case 1: *l2* is empty - do nothing. */
    return;
  } else if (CDLL_ISEMPTY(l)) {
    /* case 2; *l* is empty - copy contents of *l2* to *l*, update
     * pointers, set *l2* to empty */
    *l = *l2;
    l->prev->next = l;
    l->next->prev = l;
    CDLL_SETEMPTY(l2);
    return;
  } else {
    /* case 3: neither *l* nor *l2* is empty - splice *l2* into *l* */
    l2copy = cdll_init();
    *l2copy = *l2;
    l2copy->next->prev = l2copy;
    l2copy->prev->next = l;
    l2copy->prev = l->prev;
    l->prev->next = l2copy;
    l->prev = l2->prev; /* useful to still have *l2* around here */
    CDLL_SETEMPTY(l2);
    return;
  }
}

void cdll_free(cdll *l) {
  if (!CDLL_ISEMPTY(l)) {
    /* if *l* is empty, l->next is NULL so assignment to l->next->prev
     * results in a segfault */
    l->prev->next = l->next;
    l->next->prev = l->prev;
  }
  /* free(l->item->payload); */
  free(l);
}

void cdll_free_all(cdll *l) {
  if (CDLL_ISEMPTY(l)) {
    free(l);
  } else {
    cdll *lnext;
    while (!(CDLL_ISSINGLT(l))) {
      lnext = l->next;
      cdll_free(l);
      l = lnext;
    }
    cdll_free(l);
  }
}
