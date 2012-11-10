/*
 * cdll_test.c
 *
 * A simplistic circular doubly-linked list implementation: Tests
 *
 * Leonhard Markert, 2012
 * Licensed under Creative Commons Attribution (cc-by).
 */

#include <assert.h>	/* assert */
#include <stdbool.h>	/* bool type */
#include <stdio.h>		/* printf */
#include <stdlib.h>	/* exit */
#include <string.h>	/* strings */

#include "cdll.h"

bool check_pointers (struct cdll *l)
{
	struct cdll *ltmp;

	if (CDLL_ISEMPTY(l)) {
		printf("Empty!\n");
		return true;
	}

	for (ltmp = l->prev; l != ltmp; l = l->next) {
		if (!(l->prev->next == l && l->next->prev == l))
				return false;
	}
	return l->prev->next == l && l->next->prev == l;
}

void cdll_todot (struct cdll *l, FILE *fp)
{
	struct cdll *ltmp;

	if (CDLL_ISEMPTY(l)) return;

	fprintf(fp, "graph cdll {\n" \
				"\tlayout = circo\n" \
				"\t%d [style = filled];\n", l->item);
	for (ltmp = l->prev; l != ltmp; l = l->next) {
		/* assert(l->next->prev == l); */
		fprintf(fp, "\t%d -- %d;\n", l->item, l->next->item);
	}
	fprintf(fp, "\t%d -- %d;\n", ltmp->item, ltmp->next->item);
	fprintf(fp, "}\n");
}

void cdll_print (struct cdll *l)
{
	struct cdll *ltmp;
	if (CDLL_ISEMPTY(l)) { printf("Empty.\n"); }
	for (ltmp = l->prev; l != ltmp; l = l->next) {
		printf("\t-- %d", l->item);
	}
	printf(" -- %d --\n", ltmp->item);
}

void update_loop (struct cdll *l, char *fname)
{
	FILE *fp;
	char cmd = 'n';
	int matched, val;

	while (true) {
		matched = scanf("%c %d", &cmd, &val);
		if (matched == 2) {
				switch(cmd) {
					case 'a': cdll_insert(l, val);
										break;
					case 'd': l = cdll_delete(l, val);
				}
				cdll_print(l);
				fp = fopen(fname, "w");
				cdll_todot(l, fp);
				fclose(fp);
				if (!check_pointers(l)) {
					exit(1);
				}
		}
	}
}

int main (int argc, char **argv)
{
	struct cdll *l = cdll_init();
	struct cdll *l2 = cdll_init();
	char *fname = "out.dot\0";
	FILE *fp;

	if (argc > 1) {
		fname = argv[1];
		update_loop(l, fname);
		return 0;
	}

	cdll_insert(l, 1);
	cdll_insert(l, 2);
	cdll_insert(l, 3);
	cdll_insert(l, 4);
	cdll_insert(l, 5);

	cdll_insert(l2, 6);
	cdll_insert(l2, 7);
	cdll_insert(l2, 8);
	cdll_insert(l2, 9);
	cdll_insert(l2, 10);

	fp = fopen(fname, "w");
	cdll_todot(l, fp);
	fclose(fp);
	getchar();

	fp = fopen(fname, "w");
	cdll_todot(l2, fp);
	fclose(fp);
	getchar();

	cdll_merge(l, l2);
	assert(CDLL_ISEMPTY(l2));
	cdll_free(l2);

	fp = fopen(fname, "w");
	cdll_todot(l, fp);
	fclose(fp);

	assert(check_pointers(l));
	cdll_free_all(l);
	return 0;
}
