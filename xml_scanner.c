#include "xml_scanner.h"

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
  // free(element);
}

void deepCopyElement(Element *dest, Element *src) {
  dest->n_attributes = src->n_attributes;
  dest->capacity = src->capacity;
  dest->is_prolog = src->is_prolog;
  strncpy(dest->name, src->name, STRING_SIZE);

  dest->attributes = (Attribute *)malloc(src->capacity * sizeof(Attribute));
  if (dest->attributes == NULL) {
    perror("malloc error");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < src->n_attributes; i++) {
    strncpy(dest->attributes[i].name, src->attributes[i].name, STRING_SIZE);
    strncpy(dest->attributes[i].value, src->attributes[i].value, STRING_SIZE);
  }
}

XMLScanner *newXMLScanner() {
  XMLScanner *xml_parser;
  xml_parser = (XMLScanner *)malloc(sizeof(XMLScanner));

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

void closeXMLScanner(XMLScanner *xml_parser) {
  for (size_t i = 0; i < xml_parser->n_elements; i++) {
    closeElement(&xml_parser->elements[i]);
  }
  free(xml_parser->elements);
  free(xml_parser->file_contents);
  free(xml_parser);
}

void printFileContents(XMLScanner *p) {
  unsigned long index = p->position;
  while (index < strlen(p->file_contents)) {
    printf("%lu = '%c' -> %d\n", index, p->file_contents[index],
           p->file_contents[index]);
    index++;
  }
}

void parseContent(XMLScanner *p) {
  char curr_char;

  while (p->position < strlen(p->file_contents)) {
    char c = consumeChar(p);

    switch (c) {
    case '<':
      curr_char = p->file_contents[p->position];
      if (curr_char == '?') {
        consumeChar(p);
        processXMLProlog(p);
      } else if (curr_char == '/') {
        while (p->file_contents[p->position] != '>') {
          p->position++;
        }
        p->position++;
        break;
      } else {
        processXMLElement(p);
      }
      break;
    case '\n':
      p->line++;
      break;
    case ' ':
    case '\r':
    case '\t':
      break;
    default:
      fprintf(stderr, "error: invalid character found.");
      exit(EXIT_FAILURE);
    }
  }
}

void processXMLProlog(XMLScanner *p) {
  Element *element = newElement();
  element->is_prolog = 1;

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
  strncpy(element->name, buf, STRING_SIZE);

  memset(buf, 0, STRING_SIZE);

  while (p->file_contents[p->position] != '?') {

    while (p->file_contents[p->position] != ' ') {

      while (p->file_contents[p->position] != '=') {
        strncat(buf, &p->file_contents[p->position], sizeof(char));
        p->position++;
      }

      if (element->capacity == element->n_attributes) {
        element->capacity = element->capacity * 2;
        element->attributes =
            (Attribute *)realloc(element->attributes, element->capacity);

        if (element->attributes == NULL) {
          perror("realloc error");
          exit(EXIT_FAILURE);
        }
      }
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

  if (p->capacity == p->n_elements) {
    p->capacity = p->capacity * 2;
    p->elements = (Element *)realloc(p->elements, p->capacity);

    if (p->elements == NULL) {
      perror("realloc error");
      exit(EXIT_FAILURE);
    }
  }
  deepCopyElement(&p->elements[p->n_elements], element);
  free(element->attributes);
  free(element);
  p->n_elements++;

  p->position += 3;
}

void processXMLElement(XMLScanner *p) {
  unsigned long index = 0;
  int found_closing = 0;
  int attr_present = 1;

  Element *element = newElement();
  element->is_prolog = 0;

  char buf[STRING_SIZE];
  memset(buf, 0, STRING_SIZE);

  while (p->file_contents[p->position] != ' ') {
    if (p->file_contents[p->position] == '>') {
      attr_present = 0;
      break;
    }

    if (p->file_contents[p->position] == '/') {
      attr_present = 0;
      found_closing = 1;
      break;
    }
    strncat(buf, &p->file_contents[p->position], sizeof(char));
    p->position++;
  }
  p->position++;

  strncpy(element->name, buf, STRING_SIZE);

  memset(buf, 0, STRING_SIZE);

  if (attr_present) {
    while (p->file_contents[p->position] != '>') {

      while (p->file_contents[p->position] != ' ') {

        while (p->file_contents[p->position] != '=') {
          strncat(buf, &p->file_contents[p->position], sizeof(char));
          p->position++;
        }

        if (element->capacity == element->n_attributes) {
          element->capacity = element->capacity * 2;
          element->attributes =
              (Attribute *)realloc(element->attributes, element->capacity);

          if (element->attributes == NULL) {
            perror("realloc error");
            exit(EXIT_FAILURE);
          }
        }
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

        if (p->file_contents[p->position + 1] == '>') {
          break;
        } else if (p->file_contents[p->position + 1] == '/') {
          found_closing = 1;
          p->position++;
          break;
        } else {
          p->position++;
        }
      }
      p->position++;
    }
  }

  if (found_closing) {
    if (p->capacity == p->n_elements) {
      p->capacity = p->capacity * 2;
      p->elements = (Element *)realloc(p->elements, p->capacity);

      if (p->elements == NULL) {
        perror("realloc error");
        exit(EXIT_FAILURE);
      }
    }
    deepCopyElement(&p->elements[p->n_elements], element);
    free(element->attributes);
    free(element);
    p->n_elements++;
    p->position++;
  } else {
    index = p->position;

    while (index < strlen(p->file_contents)) {

      if (p->file_contents[index] == '/') {

        index++;
        while (p->file_contents[index] != '>') {
          strncat(buf, &p->file_contents[index], sizeof(char));
          index++;
        }

        if ((strcmp(buf, element->name)) == 0) {
          if (p->capacity == p->n_elements) {
            p->capacity = p->capacity * 2;
            p->elements = (Element *)realloc(p->elements, p->capacity);

            if (p->elements == NULL) {
              perror("realloc error");
              exit(EXIT_FAILURE);
            }
          }
          deepCopyElement(&p->elements[p->n_elements], element);
          free(element->attributes);
          free(element);
          p->n_elements++;
          break;
        }
        memset(buf, 0, STRING_SIZE);
      }
      index++;
    }
  }

  memset(buf, 0, STRING_SIZE);
  p->position++;
}

char consumeChar(XMLScanner *xml_parser) {
  return xml_parser->file_contents[xml_parser->position++];
}
