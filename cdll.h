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

/*
 * The cdll structure represents a circular doubly-linked list _and_ an
 * element of such a list at the same time.
 *
 * Note: The *item* member should actually be of type Item, and have a
 * key and a "payload", i.e. a pointer to the thing it represents. In
 * this toy implementation, *item* ist just an int that is at the same
 * time the key and the value of this node.
 */
struct cdll {
        int                item;
        struct cdll        *prev;
        struct cdll        *next;
};

/*
 * An empty element is represented by a cdll that has its *next* pointer
 * set to NULL. This is not too elegant, but works just fine.
 */
#define CDLL_SETEMPTY(l)   ((l)->next =  NULL)
#define CDLL_ISEMPTY(l)    ((l)->next == NULL)
/* A singleton cdll has itself as its *next* element. */
#define CDLL_ISSINGLT(l)   ((l)->next == (l))

struct cdll *cdll_init(void);
void cdll_insert(struct cdll *l, int i);
struct cdll *cdll_delete(struct cdll *l, int i);
void cdll_merge(struct cdll *l, struct cdll *l2);

void cdll_free(struct cdll *l);
void cdll_free_all(struct cdll *l);

#endif /* CDLL_H */
