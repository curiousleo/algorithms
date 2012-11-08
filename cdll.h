#ifndef CDLL_H
#define CDLL_H

typedef
struct cdll {
  /* cdll_item item; */
  int item;
  struct cdll *prev;
  struct cdll *next;
} cdll;

/*
typedef
struct cdll_item {
  int key;
  void *payload;
} item;
*/

#define CDLLEMPTY(l)  ((cdll *)((l)->next) == (cdll *)NULL) /* l empty? */
/* l singleton? */
#define CDLLSINGL(l)  ((cdll *)((l)->next) == (cdll *)(l))

cdll *cdll_make(void);
/* void cdll_insert(cdll *l, cdll_item i); */
void cdll_insert(cdll *l, int i);
/* void cdll_delete(cdll *l, cdll_item i); */
void cdll_delete(cdll *l, int i);
void cdll_merge(cdll *l, cdll *l2);
void cdll_free(cdll *l);
void cdll_free_all(cdll *l);

#endif /* CDLL_H */
