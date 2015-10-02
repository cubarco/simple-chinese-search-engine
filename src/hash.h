#ifndef HASH_H
#define HASH_H

#include "type.h"

Hashtable HTCreate(void);

void HTDestroy(Hashtable);

void HTInsert(Hashtable, const char *key, void *value);

void *HTSearch(Hashtable, const char *key);

void HTDelete(Hashtable, const char *key);
#endif
