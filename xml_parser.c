#include "xml_parser.h"
#include "epub_extractor.h"

XMLParser *newXMLParser() {
  XMLParser *xml_parser;

  xml_parser = (XMLParser *)malloc(sizeof(XMLParser));

  if (xml_parser == NULL) {
    perror("malloc error");
    exit(EXIT_FAILURE);
  }

  return xml_parser;
}

void closeXMLParser(XMLParser *xml_parser) {
  free(xml_parser->file_contents);
  free(xml_parser->elements);
  free(xml_parser);
}

void parseContent(XMLParser *p) {
  char curr_char;
  p->position = 0;
  p->line = 1;

  while (p->position < strlen(p->file_contents)) {
    char c = consumeChar(p);

    switch (c) {
    case '<':
      curr_char = p->file_contents[p->position];
      if (curr_char == '?') {
        consumeChar(p);
        // processXMLProlog(p);
      } else {
        processXMLAttribute(p);
      }
      break;
    default:
      fprintf(stderr, "error: invalid character found.");
      exit(EXIT_FAILURE);
    }
  }
}

void processXMLProlog(XMLParser *p) {
  char buf[BUF_SIZE];
  memset(buf, 0, BUF_SIZE);

  while (p->file_contents[p->position] != ' ') {
    strncat(buf, &p->file_contents[p->position], sizeof(char));
    p->position++;
  }
  p->position++;

  if ((strcmp(buf, "xml")) != 0) {
    fprintf(stderr, "error: invalid xml prolog tag.\n");
    exit(EXIT_FAILURE);
  }

  memset(buf, 0, BUF_SIZE);

  struct s_element *element =
      (struct s_element *)malloc(sizeof(struct s_element));
  element->n_attributes = 0;
  element->attributes =
      (struct s_attribute *)malloc(50 * sizeof(struct s_attribute));

  while (p->file_contents[p->position] != '?') {

    while (p->file_contents[p->position] != ' ') {
      while (p->file_contents[p->position] != '=') {
        strncat(buf, &p->file_contents[p->position], sizeof(char));
        p->position++;
      }
      p->position += 2;
      element->attributes[element->n_attributes].name = buf;
      memset(buf, 0, BUF_SIZE);

      while (p->file_contents[p->position] != '"') {
        strncat(buf, &p->file_contents[p->position], sizeof(char));
        p->position++;
      }
      element->attributes[element->n_attributes].value = buf;
      memset(buf, 0, BUF_SIZE);

      p->position++;
    }

    p->position++;
  }

  printf("buf: %s\n", buf);
}

void processXMLAttribute(XMLParser *xml_parser) { (void)xml_parser; }

char consumeChar(XMLParser *xml_parser) {
  return xml_parser->file_contents[xml_parser->position++];
}
