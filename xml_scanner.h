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

#include <stdbool.h>

typedef const unsigned int CUI;

// Parsing options
#define PARSE_MINIMAL ((CUI) 0x0000)
#define PARSE_PI ((CUI) 0x0001)
#define PARSE_COMMENTS ((CUI) 0x0002)
#define PARSE_CDATA ((CUI) 0X0004)
#define PARSE_WS_PCDATA ((CUI) 0X0008) // SP && MM
#define PARSE_ESCAPES ((CUI) 0X0010)
#define PARSE_EOL ((CUI) 0X0020)
#define PARSE_WCONV_ATTRIBUTE ((CUI) 0X0040)
#define PARSE_WNORM_ATTRIBUTE ((CUI) 0X0080)
#define PARSE_DECLARATION ((CUI) 0X0100)
#define PARSE_DOCTYPE ((CUI) 0X0200)
#define PARSE_WS_PCDATA_SINGLE ((CUI) 0X0400) // SP & MM
#define PARSE_TRIM_PCDATA ((CUI) 0X0800)
#define PARSE_FRAGMENT ((CUI) 0X1000)
#define PARSE_EMBED_PCDATA ((CUI) 0X2000)
#define PARSE_MERGE_PCDATA ((CUI) 0X4000)
#define PARSE_DEFAULT ((CUI) PARSE_CDATA | PARSE_ESCAPES | PARSE_WCONV_ATTRIBUTE | PARSE_EOL)
#define PARSE_FULL ((CUI) PARSE_DEFAULT | PARSE_PI | PARSE_COMMENTS | PARSE_DECLARATION | PARSE_DOCTYPE)

// Tree node types
enum xml_node_type {
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

struct s_xml_attribute;
struct s_xml_node; // xml_node_2
struct xml_node_iterator;
struct xml_attribute_iterator;
struct xml_named_node_iterator;
struct xml_tree_walker;
struct xml_parse_result;
// struct xml_node;
struct xml_text;

/*************************************************************
 *
 * XML Attribute struct
 *
 *************************************************************/

typedef struct {
  struct s_xml_attribute *_attr;
} xml_attribute;

// struct xml_attribute methods
bool emptyAttr(const xml_attribute *);

const char *name(const xml_attribute *);
const char *value(const xml_attribute *);
const char *asString(const xml_attribute *, const char* /* def */);

int asInt(const xml_attribute *, int /* def */);
unsigned int asUint(const xml_attribute *, unsigned int /* def */);
double asDouble(const xml_attribute, double /* def */);
float asFloat(const xml_attribute *, float /* def */);
bool asBool(const xml_attribute *, bool /* def */);

bool setName(xml_attribute *, const char */* rhs */);
bool setNameWithSize(xml_attribute *, const char */* rhs */, size_t /* size */);
bool setValue(xml_attribute *, const char */* rhs */);
bool setValueWithSize(xml_attribute *, const char */* rhs */, size_t /* size */);

bool setValueInt(xml_attribute *, int /* rhs */);
bool setValueUint(xml_attribute *, unsigned int /* rhs */);
bool setValueLong(xml_attribute *, long /* rhs */);
bool setValueULong(xml_attribute *, unsigned long /* rhs */);
bool setValueDouble(xml_attribute *, double /* rhs */);
bool setValueDoubleP(xml_attribute *, double /* rhs */, int /* precision */);
bool setValueFloat(xml_attribute *, float /* rhs */);
bool setValueFloatP(xml_attribute *, float /* rhs */, int /* precision */);
bool setValueBool(xml_attribute *, bool /* rhs */);

xml_attribute next_attribute(const xml_attribute *);
xml_attribute previous_attribute(const xml_attribute *);

size_t hashValue(const xml_attribute *);
struct s_xml_attribute *internalValue(const xml_attribute *);

/*************************************************************
 *
 * XML Node struct
 *
 *************************************************************/

typedef struct {
  struct s_xml_node *root;
} xml_node;

bool emptyNode(const xml_node *);
enum xml_node_type type(const xml_node *);

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
