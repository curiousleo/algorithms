/*
 * cdll.h
 *
 * A simplistic circular doubly-linked list implementation: Header
 *
 * Leonhard Markert, 2012
 * Licensed under Creative Commons Attribution (cc-by).
 *
 */

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

#define CDLL_SETEMPTY(l)  ((l)->next =  NULL)
#define CDLL_ISEMPTY(l)   ((l)->next == NULL)
#define CDLL_ISSINGLT(l)  ((l)->next == (l))

cdll *cdll_init(void);
/* void cdll_insert(cdll *l, cdll_item i); */
void cdll_insert(cdll *l, int i);
/* void cdll_delete(cdll *l, cdll_item i); */
cdll *cdll_delete(cdll *l, int i);
void cdll_merge(cdll *l, cdll *l2);

void cdll_free(cdll *l);
void cdll_free_all(cdll *l);

#endif /* CDLL_H */
