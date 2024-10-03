#ifndef __xml_scanner_h
#define __xml_scanner_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#define _NEW_API
#define STRING_SIZE 512

#if defined(_NEW_API)

typedef const unsigned int CUI;

// Tree node types
enum xm_node_type {
  node_null,
  node_document,
  node_element,
  node_pcdata,
  node_cdata,
  node_comment,
  node_pi,
  node_declaration,
  node_doctype
};

// Parsing options

CUI parse_minimal = 0x0000;
CUI parse_pi = 0x0001;
CUI parse_comments = 0x0002;
CUI parse_cdata = 0x0004;
CUI parse_ws_pcdata = 0x0008; // SP && MM
CUI parse_escapes = 0x0010;
CUI parse_eol = 0x0020;
CUI parse_wconv_attribute = 0x0040;
CUI parse_wnorm_attribute = 0x0080;
CUI parse_declaration = 0x0100;
CUI parse_doctype = 0x0200;
CUI parse_ws_pcdata_single = 0x0400;// SP & MM
CUI parse_trim_pcdata = 0x0800;
CUI parse_fragment = 0x1000;
CUI parse_embed_pcdata = 0x2000;
CUI parse_merge_pcdata = 0x4000;
CUI parse_default = parse_cdata | parse_escapes | parse_wconv_attribute | parse_eol;
CUI parse_full = parse_default | parse_pi | parse_comments | parse_declaration | parse_doctype;

// Encoding options for xml docs
enum xml_encoding {
  encoding_auto,
  encoding_utf8,
  encoding_utf16_le,
  encoding_utf16_be,
  encoding_utf16,
  encoding_utf32_le,
  encoding_utf32_be,
  encoding_utf32,
  encoding_wchar,
  encoding_latin1
};

// TODO: Declare formatting flags

struct xml_attribute;
struct xml_node; // xml_node_2
struct xml_node_iterator;
struct xml_attribute_iterator;
struct xml_named_node_iterator;
struct xml_tree_walker;
struct xml_parse_result;
struct xml_text;

#endif

typedef struct {
  char name[STRING_SIZE];
  char value[STRING_SIZE];
} Attribute;

struct Element {
  char name[STRING_SIZE];
  char *inside_value;
  size_t n_attributes;
  size_t capacity;
  Attribute *attributes;
  int is_prolog;

  int is_parent;
  struct Element *children[50];
  struct Element *parent;
};
typedef struct Element Element;


typedef struct {
  size_t position;
  size_t line;
  char *file_contents;
  size_t n_elements;
  size_t capacity;
  struct Element *elements;
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
