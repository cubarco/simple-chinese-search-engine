#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "utils.h"
#include "seg.h"

struct str_elem *_split_text(const unsigned char *text)
{
    if (!text)
        return NULL;

    struct str_elem *head = NULL;
    struct str_elem **e = &head;
    int text_len = strlen((char *)text);
    char buf[4096];
    int i, j;

    for (i = 0; i < text_len; ) {
        /* drop unvisiable charactors */
        if (text[i] < 33 || text[i] == 127) {
            i++;
            continue;
        }

        /* two byte utf8 charactor must start with byte 110XXXXX */
        if (6 == text[i] >> 5) {
            /*printf("%x%x\n", text[i], text[i+1]);*/
            buf[0] = text[i++];
            buf[1] = text[i++];
            buf[2] = '\0';
            *e = malloc(sizeof(**e));
            assert(*e);
            (*e)->str = strdup(buf);
            (*e)->next = NULL;
            e = &(*e)->next;
            continue;
        }

        /* delims, check alphabet */
        if (isdelim(text[i])) {
            buf[0] = text[i++];
            buf[1] = '\0';
            *e = malloc(sizeof(**e));
            assert(*e);
            (*e)->str = strdup(buf);
            (*e)->next = NULL;
            e = &(*e)->next;
            continue;
        }

        j = i;
        if (isda(text[i]))
            /* English words */
            while (++j < text_len && isda(text[j]));
        else
            /* 3-bytes utf8 charactors only */
            while (++j < text_len && text[j] > 127 && 6 != text[j] >> 5);

        j--;
        snprintf(buf, j - i + 2, "%s", text + i);
        *e = malloc(sizeof(**e));
        assert(*e);
        (*e)->str = strdup(buf);
        (*e)->next = NULL;
        e = &(*e)->next;
        i = j + 1;
    }

    /*
     *e = &head;
     *printf("%s\n", (*e)->str);
     *while ((e = &(*e)->next), *e)
     *    printf("%s\n", (*e)->str);
     */

    return head;
}

struct str_elem *SIMPLE_MMSEG(Hashtable dict, const unsigned char *text)
{
    if (!text)
        return NULL;

    char buf[512];
    char *chunk;
    int chunk_len;
    int start, end, i;
    int count = 0;
    struct str_elem *head = NULL;
    struct str_elem **e = &head;
    struct str_elem *chunk_head;
    struct str_elem *chunk_e;

    chunk_head = _split_text(text);
    chunk_e = chunk_head;
    do {
        chunk = chunk_e->str;
        chunk_len = strlen(chunk);

        if (isda(chunk[0]) || isdelim(chunk[0])) {
            /* English chunk */

            /* seg as one character */
            for (i = 0; i < (int)strlen(chunk); i++) {
                snprintf(buf, 2, "%s", &chunk[i]);
                *e = malloc(sizeof(**e));
                assert(*e);
                (*e)->str = strdup(buf);
                (*e)->next = NULL;
                e = &(*e)->next;
                count++;
            }

            *e = malloc(sizeof(**e));
            assert(*e);
            (*e)->str = strdup(chunk);
            (*e)->next = NULL;
            e = &(*e)->next;
            count++;
        } else {
            /*
             *for (i = 0; i < (int)strlen(chunk); i = i + 3) {
             *    snprintf(buf, 4, "%s", &chunk[i]);
             *    *e = malloc(sizeof(**e));
             *    assert(*e);
             *    (*e)->str = strdup(buf);
             *    (*e)->next = NULL;
             *    e = &(*e)->next;
             *    count++;
             *}
             */
            for (start = 0; start < chunk_len; start = end + 1) {

                *e = malloc(sizeof(**e));
                assert(*e);

                if ((end = start + MAXLEN_OF_ONEWORD) >= chunk_len - 1)
                    end = chunk_len - 1;

                for (; end > start + 2; end--) {
                    /* the size argument includes the '\0' byte */
                    snprintf(buf, end - start + 2, "%s", chunk + start);

                    if (HTSearch(dict, buf)) {
                        /* got it, then insert to the list */
                        (*e)->str = strdup(buf);
                        break;
                    }
                }

                if (end <= start + 2) {
                    snprintf(buf, 4, "%s", chunk + start);
                    (*e)->str = strdup(buf);
                }

                (*e)->next = NULL;
                e = &(*e)->next;
                count++;
            }
        }
        /*
         *chunk_tmp = chunk_e->next;
         *free(chunk_e->str);
         *free(chunk_e);
         */
    } while ((chunk_e = chunk_e->next));
    free_str_list(chunk_head);
    /*
     *e = &head;
     *printf("%s\n", (*e)->str);
     *while ((e = &(*e)->next), *e)
     *    printf("%s\n", (*e)->str);
     */

    return head;
}
