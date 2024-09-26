#ifndef __xml_parser_h
#define __xml_parser_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct s_attribute {
  char *name;
  char *value;
};

struct s_element {
  char *name;
  struct s_attribute *attributes;
  int is_prolog;
};

struct s_xmlparser {
  size_t position;
  size_t line;
  char *file_contents;
  struct s_element *elements;
};
typedef struct s_xmlparser XMLParser;

void parseContent(XMLParser *xml_parser);
void processXMLProlog(XMLParser *xml_parser);
void processXMLAttribute(XMLParser *xml_parser);

#endif
