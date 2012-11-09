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
#include <string.h>   /* memcpy */

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
  if (CDLL_ISSINGL(l)) {
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

void cdll_merge(cdll *l, cdll **l2ptr) {
  cdll *l2 = *l2ptr;
  if (CDLL_ISEMPTY(l)) {
    l = l2;
  } else if (CDLL_ISEMPTY(l2)) {
    return;
  } else {
    /* need one temporary pointer for crossover */
    cdll *lprev = l->prev;

    l->prev->next = l2;
    l2->prev->next = l;
    l->prev = l2->prev;
    l2->prev = lprev;
  }
#ifndef RELAXED_MERGE
  /* this is a bit unneccessary, but the postcondition on merge is that
   * the cdll *l2* that is merged into the "our" cdll *l1* has to be
   * empty, so we need to copy its item, make all pointers to point to
   * the copy instead of the original, and set *l2* to be empty */
  // TODO: What if l == l2?
  cdll_insert(l, l2->item);
  cdll_free(l2);
  *l2ptr = NULL;
#endif /* RELAXED_MERGE */
}

void cdll_free(cdll *l) {
  l->prev->next = l->next;
  l->next->prev = l->prev;
  /* free(l->item->payload); */
  free(l);
}

void cdll_free_all(cdll *l) {
  if (CDLL_ISEMPTY(l)) {
    free(l);
  } else {
    cdll *lnext;
    while (!(CDLL_ISSINGL(l))) {
      lnext = l->next;
      cdll_free(l);
      l = lnext;
    }
    cdll_free(l);
  }
}
