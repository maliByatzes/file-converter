#ifndef __xml_scanner_h
#define __xml_scanner_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#define STRING_SIZE 512

typedef struct {
  char name[STRING_SIZE];
  char value[STRING_SIZE];
} Attribute;

typedef struct {
  char name[STRING_SIZE];
  char *value;
  size_t n_attributes;
  size_t capacity;
  Attribute *attributes;
  int is_prolog;
} Element;

typedef struct {
  size_t position;
  size_t line;
  char *file_contents;
  size_t n_elements;
  size_t capacity;
  Element *elements;
} XMLScanner;

Element *newElement();
void closeElement(Element *);
void deepCopyElement(Element *, Element *);

XMLScanner *newXMLScanner();
void closeXMLScanner(XMLScanner *);
void parseContent(XMLScanner *);
void processXMLProlog(XMLScanner *);
void processXMLElement(XMLScanner *);
char consumeChar(XMLScanner *);

#endif
