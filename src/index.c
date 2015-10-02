#include <stdio.h>
#include <dirent.h>
#include <string.h>

#include "html.h"
#include "hash.h"
#include "seg.h"
#include "utils.h"
#include "index.h"

#define HTMLS_DIR "contents/htmls/"

struct str_elem *html_to_dict(Hashtable dst, Hashtable dict,
        const char *contents, int index, struct str_elem *str_list)
{
    char *text = malloc(65535);
    bzero(text, 65535);
    html_gettext(contents, text);

    struct str_elem *segs = SIMPLE_MMSEG(dict, (unsigned char *)text);
    struct str_elem *seg = segs;
    struct str_elem *str_tmp = str_list, *str_new;
    struct int_elem *html_index_ent = NULL, *html_index_ent_tmp = NULL;
    struct int_elem **pte = NULL;

    do {
        if (!seg->str)
            continue;

        html_index_ent_tmp = malloc(sizeof(*html_index_ent_tmp));
        html_index_ent_tmp->value = index;
        html_index_ent_tmp->next = NULL;
        if ((html_index_ent = (IntElem)HTSearch(dst, seg->str))) {
            pte = &html_index_ent;
            if (!(*pte)->next) {
                if (html_index_ent->value == index)
                    /* this index exists */
                    free(html_index_ent_tmp);
                else
                    html_index_ent->next = html_index_ent_tmp;
            } else {
                /* seek to the last two */
                while (pte = &(*pte)->next, (*pte)->next);
                if ((*pte)->value == index)
                    /* this index exists */
                    free(html_index_ent_tmp);
                else
                    (*pte)->next = html_index_ent_tmp;
            }
        } else {
            /* save the string to str list */
            str_new = malloc(sizeof(*str_new));
            str_new->str = strdup(seg->str);
            str_new->next = str_tmp;
            str_tmp = str_new;
            HTInsert(dst, seg->str, html_index_ent_tmp);
        }
    } while ((seg = seg->next));
    free_str_list(segs);
    return str_tmp;
}

void load_dict(Hashtable dict, const char *filename)
{
    char *lineptr = NULL;
    size_t ptrsize = 512;
    FILE *dict_file = fopen(filename, "r");

    while ((lineptr = malloc(512),
                getline(&lineptr, &ptrsize, dict_file)) != -1) {
        stripnl(lineptr);
        if (strlen(lineptr))
            HTInsert(dict, lineptr, lineptr);
    }
}

void save_index(struct str_elem *str_list, Hashtable html_rev_index)
{
    char buf[4096], index_buf[512];
    struct str_elem *str_ent = str_list;
    struct int_elem *int_ent = NULL;
    FILE *file = fopen("index.dat", "w");

    do {
        if (!(int_ent = (IntElem)HTSearch(html_rev_index, str_ent->str)))
            continue;
        bzero(buf, 4096);
        snprintf(buf, 4096, "%s ", str_ent->str);
        bzero(index_buf, 512);
        sprintf(index_buf, "%d", int_ent->value);
        strcat(buf, index_buf);
        while ((int_ent = int_ent->next)) {
            bzero(index_buf, 512);
            sprintf(index_buf, ",%d", int_ent->value);
            strcat(buf, index_buf);
        };
        fprintf(file, "%s\n", buf);
    } while ((str_ent = str_ent->next));
    fclose(file);
}

void load_index(Hashtable html_rev_index)
{
    FILE *dat_file = fopen("index.dat", "r");
    char *lineptr, *ip, *sp;
    struct int_elem *int_ent;
    size_t ptrsize = 4096;

    lineptr = malloc(4096);
    while ((getline(&lineptr, &ptrsize, dat_file)) != -1) {
        stripnl(lineptr);
        if (strlen(lineptr)) {
            sp = strtok(lineptr, " ");

            ip = strtok(NULL, ",");
            int_ent = malloc(sizeof(*int_ent));
            sscanf(ip, "%d", &int_ent->value);
            int_ent->next = NULL;
            HTInsert(html_rev_index, sp, int_ent);
            while ((ip = strtok(NULL, ","))) {
                int_ent->next = malloc(sizeof(*int_ent));
                int_ent = int_ent->next;
                sscanf(ip, "%d", &int_ent->value);
                int_ent->next = NULL;
            }
        }
    }
    fclose(dat_file);
}
