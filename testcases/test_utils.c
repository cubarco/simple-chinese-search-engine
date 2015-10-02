#include <stdio.h>

#include "utils.h"
#include "type.h"

int main()
{
    struct int_elem *first = malloc(sizeof(*first));
    struct int_elem *second = malloc(sizeof(*second));

    first->value = 1;
    first->next = malloc(sizeof(*first));
    first->next->value = 3;
    first->next->next = malloc(sizeof(*first));
    first->next->next->value = 2;
    first->next->next->next = NULL;

    second->value = 1;
    second->next = malloc(sizeof(*second));
    second->next->value = 5;
    second->next->next = NULL;


    struct int_elem *iter = sort_int_list(first);
    do {
        printf("%d\n", iter->value);
    } while ((iter = iter->next));

    putchar('\n');

    iter = inter(first, second);
    do {
        printf("%d\n", iter->value);
    } while ((iter = iter->next));

    putchar('\n');

    iter = merge(first, second);
    do {
        printf("%d\n", iter->value);
    } while ((iter = iter->next));

    return 0;
}
