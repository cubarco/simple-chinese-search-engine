#include <stdio.h>

#include "hash.h"
#include "index.h"
#include "search.h"
#include "utils.h"

int main()
{
    char buf[4096];
    int result;
    Hashtable rev_index = HTCreate();
    load_index(rev_index);

    struct int_elem *e, *tmp;

    for (;;) {
        putchar('>');
        putchar(' ');
        result = scanf("%[^\n]", buf);

        if (result == -1) {
            puts("\nBye");
            break;
        }

        /* then drop the newline char */
        getchar();

        e = search_by_string(rev_index, buf);
        tmp = e;
        if (!e)
            printf("not found.\n");
        else {
            printf("found: ");
            do {
                printf("%d.html ", e->value);
            } while ((e = e->next));
            putchar('\n');
            free_int_list(tmp);
        }
    }

    return 0;
}
