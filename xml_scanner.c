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
    printf("\nNew iteration....\n");
    printf("p->n_elements: %zu\n", p->n_elements);

    char c = consumeChar(p);
    printf("c: %c => %d\n", c, c);

    switch (c) {
    case '<':
      curr_char = p->file_contents[p->position];
      printf("curr_char: %c => %d\n", curr_char, curr_char);
      if (curr_char == '?') {
        consumeChar(p);
        printf("\nProcessing XML Prolog....\n");
        processXMLProlog(p);
      } else if (curr_char == '/') {
        printf("\nProcessing chars till '>'...\n");
        while (p->file_contents[p->position] != '>') {
          p->position++;
        }
        p->position++;
        break;
      } else {
        printf("\nProcessing XML Element...\n");
        processXMLElement(p);
      }
      break;
    case '\n':
      printf("\nProcessing LF...\n");
      p->line++;
      break;
    case ' ':
    case '\r':
    case '\t':
      printf("\nProcessing SPC, CR, TAB...\n");
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
  printf("element->name: %s\n", element->name);

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
      printf("element->attributes[element->n_attributes].name: %s\n", element->attributes[element->n_attributes].name);

      memset(buf, 0, STRING_SIZE);

      p->position += 2;
      while (p->file_contents[p->position] != '"') {
        strncat(buf, &p->file_contents[p->position], sizeof(char));
        p->position++;
      }

      strncpy(element->attributes[element->n_attributes].value, buf,
              STRING_SIZE);
      printf("element->attributes[element->n_attributes].value: %s\n", element->attributes[element->n_attributes].value);
      element->n_attributes++;
      printf("Added a new attribute...\n");
      printf("n_attributes: %zu\n", element->n_attributes);
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
  printf("Added a new element...\n");
  printf("p->element[p->n_elements].name: %s\n", p->elements[p->n_elements].name);
  free(element->attributes);
  free(element);
  p->n_elements++;
  printf("n_elements: %zu\n", p->n_elements);

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
  printf("element->name: %s\n", element->name);

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
        printf("element->attributes[element->n_attributes].name: %s\n", element->attributes[element->n_attributes].name);

        memset(buf, 0, STRING_SIZE);

        p->position += 2;
        while (p->file_contents[p->position] != '"') {
          strncat(buf, &p->file_contents[p->position], sizeof(char));
          p->position++;
        }

        strncpy(element->attributes[element->n_attributes].value, buf,
                STRING_SIZE);
        printf("element->attributes[element->n_attributes].value: %s\n", element->attributes[element->n_attributes].value);
        element->n_attributes++;
        printf("Added a new attribute...\n");
        printf("n_attributes: %zu\n", element->n_attributes);

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


        printf("checking %c => %d for SPC\n", p->file_contents[p->position], p->file_contents[p->position]);

        while (p->file_contents[p->position] == '\n' ||
            p->file_contents[p->position] == '\r' ||
            p->file_contents[p->position] == '\t'
        ) {
          printf("Encountered a LF or CR or TAB\n");
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
    printf("Added a new element...(inside found_closing)\n");
    printf("p->element[p->n_elements].name: %s\n", p->elements[p->n_elements].name);
    free(element->attributes);
    free(element);
    p->n_elements++;
    printf("n_elements: %zu\n", p->n_elements);
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
          found_closing = 1;
          if (p->capacity == p->n_elements) {
            p->capacity = p->capacity * 2;
            p->elements = (Element *)realloc(p->elements, p->capacity);

            if (p->elements == NULL) {
              perror("realloc error");
              exit(EXIT_FAILURE);
            }
          }
          deepCopyElement(&p->elements[p->n_elements], element);
          printf("Added a new element...(inside not found closing)\n");
          printf("p->element[p->n_elements].name: %s\n", p->elements[p->n_elements].name);
          free(element->attributes);
          free(element);
          p->n_elements++;
          printf("n_elements: %zu\n", p->n_elements);
          break;
        }
        memset(buf, 0, STRING_SIZE);
      }
      index++;
    }
  }

  printf("\nPosition: %zu\n", p->position);
  printf("char at pos: %c => %d\n", p->file_contents[p->position], p->file_contents[p->position]);

  // Consume every char from > to </{element->name}>

  memset(buf, 0, STRING_SIZE);
  char inside_value[2048];
  memset(inside_value, 0, 2048);

  if (p->file_contents[p->position] == '>') {
    while (p->position < strlen(p->file_contents)) {
      if (p->file_contents[p->position] == '/') {
        p->position++;
        while (p->file_contents[p->position] != '>') {
                }
      }

      p->position++;
    }
  }

  p->position++;
}

char consumeChar(XMLScanner *xml_parser) {
  return xml_parser->file_contents[xml_parser->position++];
}
