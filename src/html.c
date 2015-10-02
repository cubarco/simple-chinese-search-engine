#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <gumbo.h>

static void _rec_clean_text(GumboNode *node, char *dst)
{
    char *text;
    unsigned int i;
    if (node->type == GUMBO_NODE_TEXT) {
        text = (char *)node->v.text.text;
        if (strlen(text))
            strcat(dst, text);
        else
            strcat(dst, " ");
    } else if (node->type == GUMBO_NODE_ELEMENT &&
               node->v.element.tag != GUMBO_TAG_SCRIPT &&
               node->v.element.tag != GUMBO_TAG_STYLE) {
        GumboVector *children = &node->v.element.children;
        for (i = 0; i < children->length; i++) {
            _rec_clean_text((GumboNode *)children->data[i], dst);
        }
    } else
        strcat(dst, " ");
}

void html_gettext(const char *contents, char *dst)
{
    GumboOutput *output = gumbo_parse(contents);
    _rec_clean_text(output->root, dst);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
}

char *_find_title(const GumboNode *root)
{
    assert(root->type == GUMBO_NODE_ELEMENT);
    assert(root->v.element.children.length >= 2);

    const GumboVector* root_children = &root->v.element.children;
    GumboNode* head = NULL;
    for (int i = 0; i < (int)root_children->length; ++i) {
        GumboNode* child = root_children->data[i];
        if (child->type == GUMBO_NODE_ELEMENT &&
                child->v.element.tag == GUMBO_TAG_HEAD) {
            head = child;
            break;
        }
    }
    assert(head != NULL);

    GumboVector* head_children = &head->v.element.children;
    for (int i = 0; i < (int)head_children->length; ++i) {
        GumboNode* child = head_children->data[i];
        if (child->type == GUMBO_NODE_ELEMENT &&
                child->v.element.tag == GUMBO_TAG_TITLE) {
            if (child->v.element.children.length != 1)
                return NULL;
            GumboNode* title_text = child->v.element.children.data[0];
            assert(title_text->type == GUMBO_NODE_TEXT ||
                    title_text->type == GUMBO_NODE_WHITESPACE);
            return (char *)title_text->v.text.text;
        }
    }
    return NULL;

}

char *html_gettitle(const char *contents)
{
    int str_len = strlen(contents);
    GumboOutput *output = gumbo_parse_with_options(
            &kGumboDefaultOptions, contents, str_len);
    char *title = _find_title(output->root);
    if (title)
        title = strdup(title);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    return title;
}
