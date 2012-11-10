/*
 * cdll.h
 *
 * A simplistic circular doubly-linked list implementation: Header
 *
 * Leonhard Markert, 2012
 * Licensed under Creative Commons Attribution (cc-by).
 */

#ifndef CDLL_H
#define CDLL_H

struct cdll {
        int                item;
        struct cdll        *prev;
        struct cdll        *next;
};

#define CDLL_SETEMPTY(l)   ((l)->next =  NULL)
#define CDLL_ISEMPTY(l)    ((l)->next == NULL)
#define CDLL_ISSINGLT(l)   ((l)->next == (l))

struct cdll *cdll_init(void);
void cdll_insert(struct cdll *l, int i);
struct cdll *cdll_delete(struct cdll *l, int i);
void cdll_merge(struct cdll *l, struct cdll *l2);

void cdll_free(struct cdll *l);
void cdll_free_all(struct cdll *l);

#endif /* CDLL_H */
