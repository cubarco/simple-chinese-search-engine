#include <stdio.h>
#include <dirent.h>
#include <string.h>

#include "index.h"
#include "utils.h"
#include "html.h"

#define HTMLS_DIR "contents/htmls/"

int main()
{
    DIR *d;
    FILE *file;
    int index;
    char buf[512];
    char *contents = malloc(65536);
    Hashtable html_rev_index, dict;
    struct str_elem *str_list = NULL;
    struct dirent *ent;

    html_rev_index = HTCreate();
    dict = HTCreate();

    load_dict(dict, "words.txt");

    d = opendir(HTMLS_DIR);
    if (!d)
        PERROR("opendir");

    /* iterator of files in this dir */
    while ((ent = readdir(d)))
        if (ent->d_type == DT_REG) {
            /* get the integer */
            sscanf(ent->d_name, "%d.html", &index);
            printf("processing: %s\n", ent->d_name);

            /* format the file path, and get raw contents */
            strcpy(buf, HTMLS_DIR);
            strcat(buf, ent->d_name);
            file = fopen(buf, "rb");
            if (file) {
                bzero(contents, 65536);
                fread(contents, 1, 65535, file);
                fclose(file);
            } else
                PERROR("open file");
            str_list = html_to_dict(html_rev_index, dict,
                    contents, index, str_list);

            /*
             *struct str_elem *e = str_list;
             *do {
             *    puts(e->str);
             *} while ((e = e->next));
             */
        }

    save_index(str_list, html_rev_index);

    closedir(d);

    return 0;
}
