#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gumbo.h"

static char *cleantext(GumboNode* node) {
  if (node->type == GUMBO_NODE_TEXT) {
    return (char *)node->v.text.text;
  } else if (node->type == GUMBO_NODE_ELEMENT &&
             node->v.element.tag != GUMBO_TAG_SCRIPT &&
             node->v.element.tag != GUMBO_TAG_STYLE) {
    char *contents = malloc(512);
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
      char *text = cleantext((GumboNode*) children->data[i]);
      /*
       *if (i != 0 && strlen(text)) {
       *  contents.append(" ");
       *}
       */
      /*
       *contents.append(text);
       */
        strncpy(contents, text, 512);
        puts(text);
    }
    return contents;
  } else {
    return "";
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    puts("Usage: clean_text <html filename>");
    exit(EXIT_FAILURE);
  }
  const char* filename = argv[1];

  FILE *in = fopen(filename, "rb");
  /*
   *if (!in) {
   *  std::cout << "File " << filename << " not found!\n";
   *  exit(EXIT_FAILURE);
   *}
   */

  /*
   *std::string contents;
   *in.seekg(0, std::ios::end);
   *contents.resize(in.tellg());
   *in.seekg(0, std::ios::beg);
   *in.read(&contents[0], contents.size());
   *in.close();
   */
    char *contents = malloc(65535);
    fread(contents, 1, 65535, in);
  GumboOutput* output = gumbo_parse(contents);
  puts(cleantext(output->root));
  gumbo_destroy_output(&kGumboDefaultOptions, output);
}
