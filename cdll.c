/*
 * cdll.c
 *
 * A simplistic circular doubly-linked list implementation.
 *
 * Leonhard Markert, 2012
 * Licensed under Creative Commons Attribution (cc-by).
 */

#include <assert.h>	/* assert */
#include <stdlib.h>	/* malloc, free */

#include "cdll.h"

static const int CDLL_SIZE = sizeof(struct cdll);

/*
 * Initialize an empty cdll.
 */
struct cdll *cdll_init(void)
{
	struct cdll *l = (struct cdll *) malloc(CDLL_SIZE);
	CDLL_SETEMPTY(l);
	return l;
}

/*
 * Insert item *i* into cdll *l*.
 */
void cdll_insert(struct cdll *l, int i)
{
	if (CDLL_ISEMPTY(l)) {
		l->item = i;
		l->next = l->prev = l;
	} else {
		struct cdll *newl = cdll_init();
		newl->item = i;

		l->prev->next = newl;
		newl->prev = l->prev;
		l->prev = newl;
		newl->next = l;
	}
}

/*
 * Delete item *i* from cdll *l*.
 *
 * Precondition: *i* must be in *l*.
 */
struct cdll *cdll_delete(struct cdll *l, int i)
{
	struct cdll *lnext;
#ifndef NDEBUG
	struct cdll *ltmp = l;
#endif /* DEBUG */
	assert(!CDLL_ISEMPTY(l));
	if (CDLL_ISSINGLT(l)) {
		CDLL_SETEMPTY(l);
		return l;
	}
	while (l->item != i) {
		/* linear search for item */
		l = l->next;
		assert(l != ltmp);
	}
	lnext = l->next;
	cdll_free(l);
	return lnext;
}

/*
 * Merge cdll *l2* into cdll *l*.
 *
 * Postcondition: *l2* is empty.
 * Note: The calling procedure must take care of freeing *l2* itself,
 * e.g. using cdll_free():
 *
 *	cdll_merge(l, l2);
 *	cdll_free(l2);
 */
void cdll_merge(struct cdll *l, struct cdll *l2)
{
	struct cdll *l2copy;
	if (CDLL_ISEMPTY(l2)) {
		/* case 1: *l2* is empty - do nothing. */
		return;
	} else if (CDLL_ISEMPTY(l)) {
		/* case 2: *l* is empty - copy contents of *l2* to *l*,
		 * update pointers, set *l2* to empty */
		*l = *l2;
		l->prev->next = l;
		l->next->prev = l;
		CDLL_SETEMPTY(l2);
		return;
	} else {
		/* case 3: neither *l* nor *l2* is empty - splice *l2*
		 * into *l* */
		l2copy = cdll_init();
		*l2copy = *l2;
		l2copy->next->prev = l2copy;
		l2copy->prev->next = l;
		l2copy->prev = l->prev;
		l->prev->next = l2copy;
		l->prev = l2->prev;
		CDLL_SETEMPTY(l2);
		return;
	}
}

/*
 * Free the memory occupied by *l* (the element).
 *
 * Note: This splices *l* out of the list of which it is an element, and
 * frees the memory of the structure *l*. To free the memory of a whole
 * list, use cdll_free_all().
 */
void cdll_free(struct cdll *l)
{
	if (!CDLL_ISEMPTY(l)) {
		/* if *l* is empty, l->next is NULL so assignment to
		 * l->next->prev results in a segfault */
		l->prev->next = l->next;
		l->next->prev = l->prev;
	}
	/* free(l->item->payload); */
	free(l);
}

/*
 * Free the memory occupied by *l* (the list).
 *
 * Note: This frees the memory of all elements in the list represented
 * by *l*. To free only the _element_ *l*, use cdll_free().
 */
void cdll_free_all(struct cdll *l)
{
	if (CDLL_ISEMPTY(l)) {
		free(l);
	} else {
		struct cdll *lnext;
		while (!(CDLL_ISSINGLT(l))) {
			lnext = l->next;
			cdll_free(l);
			l = lnext;
		}
		cdll_free(l);
	}
}
