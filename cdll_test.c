/*
 * cdll_test.c
 *
 * A simplistic circular doubly-linked list implementation: Tests
 *
 * Leonhard Markert, 2012
 * Licensed under Creative Commons Attribution (cc-by).
 */

#include <assert.h>         /* assert */
#include <stdbool.h>        /* bool type */
#include <stdio.h>          /* printf */
#include <stdlib.h>         /* exit */
#include <string.h>         /* strings */

#include "cdll.h"

#define USAGE               "cdll_test [-i] <dotfile.dot>\n"

/*
 * Check the *prev* and *next* pointers in cdll *l* for consistency.
 */
static bool check_pointers(struct cdll *l)
{
        struct cdll *ltmp;

        if (CDLL_ISEMPTY(l)) return true;

        for (ltmp = l->prev; l != ltmp; l = l->next) {
                if (!(l->prev->next == l && l->next->prev == l))
                        return false;
        }
        return l->prev->next == l && l->next->prev == l;
}

/*
 * Write cdll *l* to file *fp* as a DOT file.
 *
 * Note: Multiple items with the same value are handled, but not
 * displayed correctly.
 */
static void cdll_todot(struct cdll *l, FILE *fp)
{
        struct cdll *ltmp;
        if (CDLL_ISEMPTY(l)) {
                fprintf(fp, "graph cdll {\n" \
                                "\tlayout = circo\n" \
                                "\tempty [style = filled];\n" \
                                "\tempty\n" \
                                "}\n");
                return;
        }

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

/*
 * Display cdll *l* as a string of values, e.g. for a cdll consisting of
 * items 1, 2 and 3 (in that order):
 *
 *         -- 1 -- 2 -- 3 --
 */
static void cdll_print(struct cdll *l)
{
        struct cdll *ltmp;
        if (CDLL_ISEMPTY(l)) {
                printf("\t[Empty]\n");
                return;
        }
        printf("\t");
        for (ltmp = l->prev; l != ltmp; l = l->next) {
                printf("-- %d ", l->item);
        }
        printf("-- %d --\n", ltmp->item);
}

/*
 * Provide a simple prompt to update a single cdll *l*, inserting or
 * deleting items one by one.
 */
static void update_loop(struct cdll *l, char *fname)
{
        FILE *fp;
        char cmd = 'n';
        int matched, val;
        printf("Interactive mode.\n" \
                        "Commands: i <value><enter> to insert <value>\n" \
                        "          d <value><enter> to delete <value>\n" \
                        "          <ctrl-c> to quit.\n");

        while (true) {
                matched = scanf("%c %d", &cmd, &val);
                if (matched == 2) {
                        switch(cmd) {
                                case 'i': cdll_insert(l, val);
                                          break;
                                case 'd': l = cdll_delete(l, val);
                        }
                        cdll_print(l);
                        fp = fopen(fname, "w");
                        cdll_todot(l, fp);
                        fclose(fp);
                }
        }
}

void print_and_pause(struct cdll *l, char *fname) {
        FILE *fp = fopen(fname, "w");
        cdll_todot(l, fp);
        fclose(fp);
        cdll_print(l);
        if (!check_pointers(l)) {
                printf("check_pointers() failed. Exiting.\n");
                exit(1);
        }
        getchar();
}

int main(int argc, char **argv)
{
        struct cdll *l = cdll_init();
        struct cdll *l2 = cdll_init();
        char *fname;

        if (argc > 2) {
                if (strcmp(argv[1], "-i") != 0) {
                        printf(USAGE);
                        return 1;
                }
                fname = argv[2];
                update_loop(l, fname);
                return 0;
        } else if (argc > 1) {
                fname = argv[1];
                printf("Testing procedure started. " \
                                "Output will be written to %s.\n" \
                                "Press any key to continue.\n",
                                fname);

                printf("empty cdll initialised.\n");
                print_and_pause(l, fname);

                cdll_insert(l, 1);
                printf("inserted 1.\n");
                print_and_pause(l, fname);

                cdll_delete(l, 1);
                printf("deleted 1.\n");
                print_and_pause(l, fname);

                cdll_insert(l, 1);
                cdll_insert(l, 2);
                cdll_insert(l, 3);
                cdll_insert(l, 4);
                cdll_insert(l, 5);
                printf("inserted 1, 2, 3, 4, 5.\n");
                print_and_pause(l, fname);

                cdll_delete(l, 3);
                printf("deleted 3.\n");
                print_and_pause(l, fname);

                cdll_insert(l2, 6);
                cdll_insert(l2, 7);
                cdll_insert(l2, 8);
                cdll_insert(l2, 9);
                cdll_insert(l2, 10);
                printf("inserted 6, 7, 8, 9, 10 to other cdll.\n");
                print_and_pause(l2, fname);

                cdll_merge(l, l2);
                printf("merged other cdll into original one.\n");
                printf("original cdll after merge:\n");
                print_and_pause(l, fname);
                printf("other cdll after merge:\n");
                print_and_pause(l2, fname);

                cdll_free(l2);
                cdll_free_all(l);
                return 0;
        } else {
                printf(USAGE);
                return 1;
        }
}
