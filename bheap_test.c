#include "bheap.h"

int
main (void)
{
  const int SIZE = 10;
  int a[] = {4,1,3,2,16,9,10,14,8,7};

  bheap *h = bheap_make(a, SIZE);
  bheap_consume(h);
  bheap_free(h);

  return 0;
}
