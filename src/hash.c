#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "hash.h"

#define INITIAL_SIZE 1024
#define GROWTH_FACTOR 2
#define HASH_MULTIPLIER 97

Hashtable _inter_ht_create(int size)
{
    int i;
    Hashtable ht = malloc(sizeof(*ht));

    assert(ht);

    ht->size = size;
    ht->n = 0;
    ht->table = malloc(sizeof(HashElem) * ht->size);

    assert(ht->table);

    for (i = 0; i < ht->size; i++)
        ht->table[i] = 0;

    return ht;
}

Hashtable HTCreate(void)
{
    return _inter_ht_create(INITIAL_SIZE);
}

void HTDestroy(Hashtable ht)
{
    int i;
    struct hash_elem *e;
    struct hash_elem *next;

    for (i = 0; i < ht->size; i++) {
        for (e = ht->table[i]; e; e = next) {
            next = e->next;
            free(e->key);
            /* Don't free values, leave it to developers */
            /*free(e->value);*/
            free(e);
        }
    }
    free(ht->table);
    free(ht);
}

static unsigned long hash_function(const char *s)
{
    unsigned const char *us;
    unsigned long h = 0;

    for (us = (unsigned const char *)s; *us; us++)
        h = h * HASH_MULTIPLIER + *us;

    return h;
}

static void grow(Hashtable ht)
{
    Hashtable httmp;
    struct hashtable swap;
    struct hash_elem *e;
    int i;

    httmp = _inter_ht_create(ht->size * GROWTH_FACTOR);

    for (i = 0; i < ht->size; i++)
        for (e = ht->table[i]; e; e = e->next)
            HTInsert(httmp, e->key, e->value);

    swap = *ht;
    *ht = *httmp;
    *httmp = swap;

    HTDestroy(httmp);
}

void HTInsert(Hashtable ht, const char *key, void *value)
{
    struct hash_elem *e;
    unsigned long h;

    assert(key);
    assert(value);

    e = malloc(sizeof(*e));

    assert(e);

    e->key = strdup(key);
    e->value = value;

    h = hash_function(key) % ht->size;

    e->next = ht->table[h];
    ht->table[h] = e;

    ht->n++;

    if (ht->n >= ht->size)
        grow(ht);
}

void *HTSearch(Hashtable ht, const char *key)
{
    struct hash_elem *e;
    for (e = ht->table[hash_function(key) % ht->size]; e; e = e->next)
        if (strcmp(e->key, key) == 0)
            return e->value;

    return NULL;
}

void HTDelete(Hashtable ht, const char *key)
{
    struct hash_elem **prev;
    struct hash_elem *e;

    for (prev = &(ht->table[hash_function(key) % ht->size]); *prev;
            prev = &((*prev)->next)) {
        if (strcmp((*prev)->key, key) == 0) {
            e = *prev;
            *prev = e->next;

            free(e->key);
            free(e->value);
            free(e);

            return;
        }
    }
}
