#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "seg.h"
#include "hash.h"
#include "utils.h"

int main(int argc, char **argv)
{
    Hashtable ht = HTCreate();
    char *text = strdup(argv[1]);
    char *lineptr = malloc(512);
    size_t ptrsize = 512, linesize = 512;

    FILE *dict_file = fopen("./words.txt", "r");

    while ((linesize = getline(&lineptr, &ptrsize, dict_file)) != -1) {
        stripnl(lineptr);
        /*
         *printf("%s, %ld\n", lineptr, linesize);
         */
        HTInsert(ht, lineptr, lineptr);
    }

    printf("hashtable created\n");

    SIMPLE_MMSEG(ht, text);

    return 0;
}
