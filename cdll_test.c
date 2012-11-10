/*
 * cdll_test.c
 *
 * A simplistic circular doubly-linked list implementation: Tests
 *
 * Leonhard Markert, 2012
 * Licensed under Creative Commons Attribution (cc-by).
 */

#include <assert.h>        /* assert */
#include <stdbool.h>        /* bool type */
#include <stdio.h>                /* printf */
#include <stdlib.h>        /* exit */
#include <string.h>        /* strings */

#include "cdll.h"

#define USAGE           "cdll_test [-i] <dotfile.dot>\n"

static bool check_pointers (struct cdll *l)
{
        struct cdll *ltmp;

        if (CDLL_ISEMPTY(l)) return true;

        for (ltmp = l->prev; l != ltmp; l = l->next) {
                if (!(l->prev->next == l && l->next->prev == l))
                        return false;
        }
        return l->prev->next == l && l->next->prev == l;
}

static void cdll_todot (struct cdll *l, FILE *fp)
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

static void cdll_print (struct cdll *l)
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

static void update_loop (struct cdll *l, char *fname)
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

int main (int argc, char **argv)
{
        struct cdll *l = cdll_init();
        struct cdll *l2 = cdll_init();
        char *fname;
        FILE *fp;

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
                printf("cdll 1 written to %s. " \
                                "Press any key to continue.\n",
                                fname);
                cdll_print(l);
                getchar();

                fp = fopen(fname, "w");
                cdll_todot(l2, fp);
                fclose(fp);
                printf("cdll 2 written to %s. " \
                                "Press any key to continue.\n",
                                fname);
                cdll_print(l2);
                getchar();

                cdll_merge(l, l2);
                assert(CDLL_ISEMPTY(l2));
                cdll_free(l2);

                fp = fopen(fname, "w");
                cdll_todot(l, fp);
                printf("merge of cdll 1 and cdll 2 written to %s.\n",
                                fname);
                cdll_print(l);
                fclose(fp);

                assert(check_pointers(l));
                cdll_free_all(l);
                return 0;
        } else {
                printf(USAGE);
                return 1;
        }
}
