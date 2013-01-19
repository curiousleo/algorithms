#include <string.h> /* needed for size_t definition */

#define PARENT(i)       (i-1)/2
#define LEFT(i)         2*(i) + 1
#define RIGHT(i)        2*(i) + 2
#define SWAP(i, j, k)   (k) = (i), (i) = (j), (j) = (k)

typedef
struct bheap
{
  size_t size;
  size_t alloc;
  int *keys;
} bheap;


bheap *bheap_make (int a[], size_t size);
bheap *bheap_build (bheap *h);
bheap *bheap_ify (bheap *h, size_t i);
bheap *bheap_insert (bheap *h, int key);
bheap *bheap_dec (bheap *h, size_t i, int key);
int bheap_peek (bheap *h);
int bheap_pop (bheap *h);
void bheap_consume (bheap *h);
void bheap_free (bheap *h);
