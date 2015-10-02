#ifndef TYPE_H
#define TYPE_H

struct int_elem {
    int value;
    struct int_elem *next;
};

typedef struct int_elem *IntElem;

struct str_elem {
    char *str;
    struct str_elem *next;
};

typedef struct str_elem *StrElem;

struct hash_elem {
    struct hash_elem *next;
    char *key;
    void *value;
};

typedef struct hash_elem *HashElem;

struct hashtable {
    int size;
    int n;
    struct hash_elem **table;
};

typedef struct hashtable *Hashtable;

#endif
