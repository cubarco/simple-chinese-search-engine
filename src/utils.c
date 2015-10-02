#include <stdio.h>
#include <string.h>

#include "utils.h"

void stripnl(char *str)
{
    int i = strlen(str);
    if (!str)
        return;
    while (str[--i] == '\n')
        str[i] = '\0';
}

int isdelim(unsigned char c)
{
    if ((c >= 33 && c <= 47) ||
            (c >= 58 && c <= 64) ||
            (c >= 91 && c <= 96) ||
            (c >= 123 && c <= 126))
        return 1;
    return 0;
}

int isda(unsigned char c)
{
    if ((c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z'))
        return 1;
    return 0;
}

struct int_elem *copy_int_list(struct int_elem *list)
{
    if (!list)
        return NULL;

    struct int_elem *ret_iter = malloc(sizeof(*ret_iter)), *head = ret_iter,
                    *iter;
    iter = list;
    ret_iter->value = iter->value;
    ret_iter->next = NULL;
    while ((iter = iter->next)) {
        ret_iter->next = malloc(sizeof(*ret_iter));
        ret_iter = ret_iter->next;
        ret_iter->value = iter->value;
        ret_iter->next = NULL;
    };
    return head;
}

struct int_elem *sort_int_list(struct int_elem *list)
{
    if (list == NULL || list->next == NULL)
        return list;

    struct int_elem *curr=list, *smallest=list,
                    *smallest_prev=list, *prev=list;

    while (curr) {
        if (curr->value < smallest->value) {
            smallest_prev = prev;
            smallest = curr;
        }
        prev = curr;
        curr = curr->next;
    }

    struct int_elem *tmp;
    if (smallest != list) {
        smallest_prev->next = list;
        tmp = list->next;
        list->next = smallest->next;
        smallest->next = tmp;
    }

    smallest->next = sort_int_list(smallest->next);

    return smallest;
}

struct int_elem *_inter(struct int_elem *first, struct int_elem *second)
{
    /* first list and second are both sorted of ascending order */
    struct int_elem *head = NULL;
    struct int_elem **ret_ins = &head;
    struct int_elem *iter_first = first, *iter_second = second;
    struct int_elem *iter_tmp;

    if (!first || !second)
        goto OUT;

    do {
        iter_tmp = iter_second;
        do {
            if (iter_first->value < iter_second->value) {
                iter_second = iter_tmp;
                break;
            }
            if (iter_first->value == iter_second->value) {
                *ret_ins = malloc(sizeof(**ret_ins));
                (*ret_ins)->value = iter_first->value;
                (*ret_ins)->next = NULL;
                ret_ins = &(*ret_ins)->next;
                iter_second = iter_second->next;
                break;
            }
            if (iter_second->next == NULL &&
                    iter_first->value > iter_second->value)
                goto OUT;
        } while ((iter_second = iter_second->next));
    } while ((iter_first = iter_first->next) && iter_second);

OUT:
    return head;
}

struct int_elem *inter(struct int_elem *first, struct int_elem *second)
{
    struct int_elem *first_copy = copy_int_list(first);
    struct int_elem *second_copy = copy_int_list(second);
    struct int_elem *ret_list = NULL;
    first_copy = sort_int_list(first_copy);
    second_copy = sort_int_list(second_copy);
    ret_list = _inter(first_copy, second_copy);
    free_int_list(first_copy);
    free_int_list(second_copy);
    return ret_list;
}

struct int_elem *_merge(struct int_elem *first, struct int_elem *second)
{
    /* first list and second are both sorted of ascending order */
    struct int_elem *result = NULL;
    struct int_elem **pr = &result, *iter=NULL;

    if (!first) {
        iter = second;

        if (!second)
            return NULL;

        do {
            *pr = malloc(sizeof(**pr));
            (*pr)->value = iter->value;
            (*pr)->next = NULL;
            pr = &(*pr)->next;
        } while ((iter = iter->next));
        return result;
    } else if (!second) {
        iter = first;

        if (!first)
            return NULL;

        do {
            *pr = malloc(sizeof(**pr));
            (*pr)->value = iter->value;
            (*pr)->next = NULL;
            pr = &(*pr)->next;
        } while ((iter = iter->next));
        return result;
    }

    if (first->value < second->value) {
        result = malloc(sizeof(*result));
        result->value = first->value;
        result->next = merge(first->next, second);
    } else if (first->value > second->value) {
        result = malloc(sizeof(*result));
        result->value = second->value;
        result->next = merge(first, second->next);
    } else {
        result = malloc(sizeof(*result));
        result->value = first->value;
        result->next = merge(first->next, second->next);
    }

    return result;
}

struct int_elem *merge(struct int_elem *first, struct int_elem *second)
{
    struct int_elem *first_copy = copy_int_list(first);
    struct int_elem *second_copy = copy_int_list(second);
    struct int_elem *ret_list = NULL;
    first_copy = sort_int_list(first_copy);
    second_copy = sort_int_list(second_copy);
    ret_list = _merge(first_copy, second_copy);
    free_int_list(first_copy);
    free_int_list(second_copy);
    return ret_list;
}

void free_int_list(struct int_elem *list)
{
    if (!list)
        return;

    struct int_elem *pl = list, *tmp;
    do {
        tmp = pl->next;
        free(pl);
    } while ((pl = tmp));
}

void free_str_list(struct str_elem *list)
{
    if (!list)
        return;

    struct str_elem *pl = list, *tmp;
    do {
        tmp = pl->next;
        free(pl->str);
        free(pl);
    } while ((pl = tmp));
}

