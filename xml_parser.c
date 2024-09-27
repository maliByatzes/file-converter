#include "xml_parser.h"

Element *newElement() {
  Element *element;
  element = (Element *)malloc(sizeof(Element));

  if (element == NULL) {
    perror("malloc error");
    exit(EXIT_FAILURE);
  }

  element->attributes = (Attribute *)malloc(50 * sizeof(Attribute));
  element->n_attributes = 0;
  element->capacity = 50;

  return element;
}

void closeElement(Element *element) {
  free(element->attributes);
  free(element);
}

XMLParser *newXMLParser() {
  XMLParser *xml_parser;
  xml_parser = (XMLParser *)malloc(sizeof(XMLParser));

  if (xml_parser == NULL) {
    perror("malloc error");
    exit(EXIT_FAILURE);
  }

  xml_parser->elements = (Element *)malloc(50 * sizeof(Element));
  xml_parser->n_elements = 0;
  xml_parser->capacity = 50;
  xml_parser->position = 0;
  xml_parser->line = 1;

  return xml_parser;
}

void closeXMLParser(XMLParser *xml_parser) {
  free(xml_parser->file_contents);
  free(xml_parser->elements);
  free(xml_parser);
}

void parseContent(XMLParser *p) {
  char curr_char;

  while (p->position < strlen(p->file_contents)) {
    char c = consumeChar(p);

    switch (c) {
    case '<':
      curr_char = p->file_contents[p->position];
      if (curr_char == '?') {
        consumeChar(p);
        processXMLProlog(p);
      } else {
        consumeChar(p);
        processXMLAttribute(p);
      }
      break;
    case '\n':
      p->line++;
      break;
    default:
      fprintf(stderr, "error: invalid character found.");
      exit(EXIT_FAILURE);
    }
  }
}

void processXMLProlog(XMLParser *p) {
  char buf[STRING_SIZE];
  memset(buf, 0, STRING_SIZE);

  while (p->file_contents[p->position] != ' ') {
    strncat(buf, &p->file_contents[p->position], sizeof(char));
    p->position++;
  }
  p->position++;

  if ((strcmp(buf, "xml")) != 0) {
    fprintf(stderr, "error: invalid xml prolog tag.\n");
    exit(EXIT_FAILURE);
  }

  memset(buf, 0, STRING_SIZE);

  Element *element = newElement();
  element->is_prolog = 1;
  strncpy(element->name, "xml", 4);

  while (p->file_contents[p->position] != '?') {

    while (p->file_contents[p->position] != ' ') {

      while (p->file_contents[p->position] != '=') {
        strncat(buf, &p->file_contents[p->position], sizeof(char));
        p->position++;
      }

      // check capacity == n_attributes
      strncpy(element->attributes[element->n_attributes].name, buf,
              STRING_SIZE);

      memset(buf, 0, STRING_SIZE);

      p->position += 2;
      while (p->file_contents[p->position] != '"') {
        strncat(buf, &p->file_contents[p->position], sizeof(char));
        p->position++;
      }

      strncpy(element->attributes[element->n_attributes].value, buf,
              STRING_SIZE);
      element->n_attributes++;
      memset(buf, 0, STRING_SIZE);

      if (p->file_contents[p->position + 1] != '?')
        p->position++;
      else
        break;
    }
    p->position++;
  }

  // check capacity == n_elements
  memcpy(&p->elements[p->n_elements], element, sizeof(Element));
  p->n_elements++;

  closeElement(element);
  p->position += 3;
}

void processXMLAttribute(XMLParser *xml_parser) {
  (void)xml_parser;
  assert(0 && "Not implemented.");
}

char consumeChar(XMLParser *xml_parser) {
  return xml_parser->file_contents[xml_parser->position++];
}
