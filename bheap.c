#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bheap.h"

bheap *
bheap_make (int a[], size_t size)
{
  int bytes = size * sizeof(int);
  bheap *h = (bheap *) malloc(sizeof(bheap));

  h->size = size;
  /* allocate twice the memory array *a* requires so that we won't need
   * a reallocation on the first insert */
  h->alloc = bytes << 1;
  h->keys = (int *) malloc(h->alloc);

  /* copy ints from *a* to h->keys */
  memmove(h->keys, a, bytes);

  return bheap_build(h);
}

/* aka *build-min-heap* */
bheap *
bheap_build (bheap *h)
{
  int i;
  for (i = ((h->size) >> 1) - 1; i >= 0; i--) {
    bheap_ify(h, i);
  }
  return h;
}

bheap *
bheap_ify (bheap *h, size_t i)
{
  size_t l, r, largest, tmp;

  for (;;) {
    l = LEFT(i); r = RIGHT(i);

    if (l < h->size && h->keys[l] < h->keys[i]) {
      largest = l;
    } else {
      largest = i;
    }

    if (r < h->size && h->keys[r] < h->keys[largest]) {
      largest = r;
    }

    if (largest != i) {
      /* exchange A[i] with A[largest] using tmp as a temp. variable */
      SWAP(h->keys[i], h->keys[largest], tmp);
      i = largest;
    } else {
      break;
    }
  }
  return h;
}

bheap *
bheap_insert (bheap *h, int key)
{
  h->size++;
  if (h->size * sizeof(int) > h->alloc) {
    /* allocate more memory for the keys */
    h->alloc <<= 1;
    h->keys = (int *) realloc(h->keys, h->alloc);
  }
  h->keys[h->size-1] = INT_MAX;
  bheap_dec(h, h->size-1, key);

  return h;
}

bheap *
bheap_dec (bheap *h, size_t i, int key)
{
  int tmp, p = PARENT(i);
  assert(key < h->keys[i]);

  h->keys[i] = key;
  while (i > 0 && h->keys[p] > h->keys[i]) {
    SWAP(h->keys[i], h->keys[p], tmp);
    i = p;
    p = PARENT(i);
  }

  return h;
}

/* aka *first*, *minimum* */
int
bheap_peek (bheap *h)
{
  assert(h->size > 0);

  return h->keys[0];
}

/* aka *extract-min* */
int
bheap_pop (bheap *h)
{
  int max = h->keys[0];
  assert(h->size > 0);

  h->keys[0] = h->keys[h->size-1];
  h->size--;

  if (h->size < (h->alloc/sizeof(int)) >> 2) {
    /* deallocate some memory */
    h->alloc >>= 1;
    h->keys = realloc(h->keys, h->alloc);
  }
  bheap_ify(h, 0);
  return max;
}

void
bheap_consume (bheap *h)
{
  while (h->size > 0) {
    printf("%d ", bheap_pop(h));
  }
  printf("\n");
}

void
bheap_free (bheap *h)
{
  free(h->keys);
  free(h);
}
