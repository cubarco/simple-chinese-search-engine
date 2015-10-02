#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

#include "type.h"

#define PERROR(s) do {perror(s); exit(1);} while (0)

void stripnl(char *str);

int isdelim(unsigned char);

/* is digit or alpha */
int isda(unsigned char);

struct int_elem *sort_int_list(struct int_elem *);
struct int_elem *copy_int_list(struct int_elem *);
struct int_elem *inter(struct int_elem *first, struct int_elem *second);
struct int_elem *merge(struct int_elem *first, struct int_elem *second);

void free_int_list(struct int_elem *);
void free_str_list(struct str_elem *);

#endif
