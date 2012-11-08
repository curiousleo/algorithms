#include <stdio.h>    /* printf */

#include "cdll.h"

int test_pointers (cdll *l) {
  if (CDLLEMPTY(l)) {
    return 1;
  } else {
    cdll *ltmp;
    for (ltmp = l->prev; l != ltmp; l = l->next) {
      if (!(l->prev->next == l && l->next->prev == l))
        return 1;
    }
    return l->prev->next == l && l->next->prev == l;
  }
}

int main () {
  return 0;
}
