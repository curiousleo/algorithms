#include <assert.h>   /* assert */
#include <stdlib.h>   /* malloc, free */

#include "cdll.h"

static const int CDLL_SIZE = sizeof(cdll);

cdll *cdll_make(void) {
  cdll *l = (cdll *) malloc(CDLL_SIZE);
  l->next = NULL;
  return l;
}

/* void cdll_insert(cdll *l, item i); */
void cdll_insert(cdll *l, int i) {
  if (CDLLEMPTY(l)) {
    l->item = i;
    l->next = l->prev = l;
  } else {
    cdll *newl = cdll_make();
    newl->item = i;

    l->next->prev = newl;
    newl->next = l->next;
    l->next = newl;
    newl->prev = l;
  }
}

/* void cdll_delete(cdll *l, item i); */
void cdll_delete(cdll *l, int i) {
#ifndef NDEBUG
  cdll *ltmp = l;
#endif /* NDEBUG */
  assert(!CDLLEMPTY(l));
  while (l->item != i) {
    /* linear search for item */
    l = l->next;
    assert(l != ltmp);
  }
  cdll_free(l);
}

void cdll_merge(cdll *l, cdll *l2) {
  /* TODO: check postcondition "l2 is empty" */
  if (CDLLEMPTY(l)) {
    l = l2;
  } else if (CDLLEMPTY(l2)) {
    return;
  } else {
    /* need one temporary pointer for crossover */
    cdll *lprev = l->prev;

    l->prev->next = l2;
    l2->prev->next = l;
    l->prev = l2->prev;
    l2->prev = lprev;
  }
}

void cdll_free(cdll *l) {
  l->prev->next = l->next;
  l->next->prev = l->prev;
  /* free(l->item->payload); */
  free(l);
}

void cdll_free_all(cdll *l) {
  if (CDLLEMPTY(l)) {
    free(l);
  } else {
    cdll *lnext;
    while (!(CDLLSINGL(l))) {
      lnext = l->next;
      cdll_free(l);
      l = lnext;
    }
    cdll_free(l);
  }
}
