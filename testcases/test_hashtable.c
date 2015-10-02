#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "hash.h"
#include "utils.h"

int
main()
{
    Hashtable ht;
    char buf[512];
    char *lineptr;
    size_t ptrsize = 512;
    size_t linesize = 0;
    char *str;
    int i = 0;
    long _time;

    ht = HTCreate();
    FILE *dict_file = fopen("./words.txt", "r");

    while ((lineptr = malloc(512),
                getline(&lineptr, &ptrsize, dict_file)) != -1) {
        stripnl(lineptr);
        /*
         *printf("%s, %ld\n", lineptr, linesize);
         */
        HTInsert(ht, lineptr, lineptr);
    }
    _time = clock();
    str = (char *)HTSearch(ht, "c#");
    if (str) {
        puts(str);
    }
    printf("time used: %ld\n", clock() - _time);

/*
 *    DictInsert(d, "foo", "hello world");
 *    puts(DictSearch(d, "foo"));
 *    DictInsert(d, "foo", "hello world2");
 *    puts(DictSearch(d, "foo"));
 *    DictDelete(d, "foo");
 *    puts(DictSearch(d, "foo"));
 *    DictDelete(d, "foo");
 *    assert(DictSearch(d, "foo") == 0);
 *    DictDelete(d, "foo");
 *
 *    for(i = 0; i < 349050; i++) {
 *        sprintf(buf, "%d", i);
 *        DictInsert(d, buf, buf);
 *    }
 *    for (i=0; i < 349050; i++) {
 *        sprintf(buf, "%d", i);
 *        puts(DictSearch(d, buf));
 *    }
 */

    HTDestroy(ht);

    return 0;
}


