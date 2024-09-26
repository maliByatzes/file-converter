#include "xml_parser.h"

void parseContent(XMLParser *p) {
  char next_char;
  p->position = 0;
  p->line = 1;

  while (p->position < strlen(p->file_contents)) {
    char c = p->file_contents[p->position];

    switch (c) {
    case '<':
      next_char = p->file_contents[p->position + 1];
      if (next_char == '?') {
        printf("<?\n");
        processXMLProlog(p);
      } else {
        printf("<\n");
        processXMLAttribute(p);
      }
      break;
    default:
      fprintf(stderr, "error: invalid character found.");
      exit(EXIT_FAILURE);
    }

    p->position++;
  }
}

void processXMLProlog(XMLParser *xml_parser) { (void)xml_parser; }

void processXMLAttribute(XMLParser *xml_parser) { (void)xml_parser; }
