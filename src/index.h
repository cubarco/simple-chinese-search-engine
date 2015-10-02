#ifndef INDEX_H
#define INDEX_H

#include "hash.h"
#include "type.h"

void load_index(Hashtable);
void load_dict(Hashtable dict, const char *filename);
void save_index(struct str_elem *str_list, Hashtable html_rev_index);
struct str_elem *html_to_dict(Hashtable dst, Hashtable dict,
        const char *contents, int index, struct str_elem *str_list);

#endif
