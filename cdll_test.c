#include <assert.h>   /* assert */
#include <stdio.h>    /* printf */
#include <stdbool.h>  /* bool type */

#include "cdll.h"

bool check_pointers (cdll *l) {
  if (CDLLEMPTY(l)) {
    return true;
  } else {
    cdll *ltmp;
    for (ltmp = l->prev; l != ltmp; l = l->next) {
      if (!(l->prev->next == l && l->next->prev == l))
        return true;
    }
    return l->prev->next == l && l->next->prev == l;
  }
}

void cdll_todot (cdll *l, FILE *fp) {
  cdll *ltmp;
  fprintf(fp, "digraph cdll {\n\tlayout = circo\n");
  for (ltmp = l->prev; l != ltmp; l = l->next) {
    fprintf(fp, "\t%d -> %d [label = \"n\"];\n",
        l->item, l->next->item);
    fprintf(fp, "\t%d -> %d [label = \"p\"];\n",
        l->item, l->prev->item);
  }
  fprintf(fp, "\t%d -> %d [label = \"n\"];\n",
      ltmp->item, ltmp->next->item);
  fprintf(fp, "\t%d -> %d [label = \"p\"];\n",
      ltmp->item, ltmp->prev->item);
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
  cdll_todot(l, stdout);
  cdll_delete(l, 3);
  cdll_todot(l, stdout);
  assert(check_pointers(l));
  cdll_free_all(l);
  return 0;
}
