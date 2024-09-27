#ifndef __xml_parser_h
#define __xml_parser_h

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

// TODO: make a custom dynamic array...

typedef struct {
  char name[STRING_SIZE];
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
} XMLParser;

Element *newElement();
void closeElement(Element *);

XMLParser *newXMLParser();
void closeXMLParser(XMLParser *);
void parseContent(XMLParser *);
void processXMLProlog(XMLParser *);
void processXMLAttribute(XMLParser *);
char consumeChar(XMLParser *);

#endif
