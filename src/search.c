#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "utils.h"
#include "index.h"

struct int_elem *search_by_string(Hashtable rev_index, const char *str)
{
    struct int_elem *ret_list, *prev_list;
    char *sp;
    char *lstr = strdup(str);

    sp = strtok(lstr, " ");
    ret_list = copy_int_list(HTSearch(rev_index, sp));
    while ((sp = strtok(NULL, " "))) {
        prev_list = ret_list;
        if (0 == strcmp(sp, "AND")) {
            if ((sp = strtok(NULL, " "))) {
                ret_list = inter(ret_list, HTSearch(rev_index, sp));
            };
        } else if (0 == strcmp(sp, "OR")) {
            if ((sp = strtok(NULL, " "))) {
                ret_list = merge(ret_list, HTSearch(rev_index, sp));
            };
        } else {
            ret_list = inter(ret_list, HTSearch(rev_index, sp));
        }
        free_int_list(prev_list);
    };
    if (lstr)
        free(lstr);
    return ret_list;
}
