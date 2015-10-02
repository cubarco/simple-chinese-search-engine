#ifndef SEG_H
#define SEG_H

#include "hash.h"
#include "type.h"

#define MAXLEN_OF_ONEWORD 48

struct str_elem *SIMPLE_MMSEG(Hashtable dict, const unsigned char *text);

#endif
