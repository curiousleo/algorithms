#include <assert.h>   /* assert */
#include <stdio.h>    /* printf */
#include <stdbool.h>  /* bool type */

#include "cdll.h"

bool check_pointers (cdll *l) {
  cdll *ltmp;

  if (CDLLEMPTY(l)) return true;

  for (ltmp = l->prev; l != ltmp; l = l->next) {
    if (!(l->prev->next == l && l->next->prev == l))
      return false;
  }
  return l->prev->next == l && l->next->prev == l;
}

void cdll_todot (cdll *l, FILE *fp) {
  cdll *ltmp;

  if (CDLLEMPTY(l)) return;

  fprintf(fp, "graph cdll {\n" \
      "\tlayout = circo\n" \
      "\tnode [shape = box];\n" \
      "\t%d [style = filled];\n", l->item);
  for (ltmp = l->prev; l != ltmp; l = l->next) {
    assert(l->next->prev == l);
    fprintf(fp, "\t%d -- %d;\n", l->item, l->next->item);
  }
  fprintf(fp, "\t%d -- %d;\n", ltmp->item, ltmp->next->item);
  fprintf(fp, "}\n");
}

int main () {
  cdll *l = cdll_make();
  assert(check_pointers(l));
  cdll_insert(l, 1);
  cdll_insert(l, 2);
  cdll_insert(l, 3);
  cdll_insert(l, 4);
  cdll_insert(l, 5);
  cdll_insert(l, 6);
  cdll_insert(l, 7);
  cdll_insert(l, 8);
  cdll_insert(l, 9);
  cdll_insert(l, 10);
  cdll_todot(l, stdout);
  assert(check_pointers(l));
  cdll_free_all(l);
  return 0;
}
