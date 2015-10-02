#include <stdio.h>
#include <stdlib.h>

#include "search.h"
#include "index.h"

int main()
{
    Hashtable rev_index = HTCreate();
    load_index(rev_index);

    struct int_elem *e;

    e = search_by_string(rev_index, "About AND 日本 AND NYAN");
    do {
        printf("%d\n", e->value);
    } while ((e = e->next));

    return 0;
}
