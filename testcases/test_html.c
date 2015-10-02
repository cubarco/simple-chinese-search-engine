#include <stdio.h>
#include <stdlib.h>

#include "html.h"

int main(int argc, char **argv)
{
    const char *filename = argv[1];
    FILE *in = fopen(filename, "rb");
    char *contents, *dst;
    contents = malloc(65535);
    dst = malloc(65535);

    fread(contents, 1, 65535, in);

    html_gettext(contents, dst);
    puts(dst);

    return 0;
}
