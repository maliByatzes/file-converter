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
typedef enum {
  node_null,
  node_document,
  node_element,
  node_pcdata,
  node_cdata,
  node_comment,
  node_pi,
  node_declaration,
  node_doctype
} xml_node_type;


// Encoding options for xml docs
typedef enum {
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
} xml_encoding;

// TODO: Declare formatting flags

struct s_xml_attribute;
struct s_xml_node;
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
bool validAttr(const xml_attribute *);

const char *getNameAttr(const xml_attribute *);
const char *getValueAttr(const xml_attribute *);
const char *asStringAttr(const xml_attribute *, const char* /* def */);

int asInt(const xml_attribute *, int /* def */);
unsigned int asUint(const xml_attribute *, unsigned int /* def */);
double asDouble(const xml_attribute, double /* def */);
float asFloat(const xml_attribute *, float /* def */);
bool asBool(const xml_attribute *, bool /* def */);

bool setNameAttr(xml_attribute *, const char */* rhs */);
bool setNameWithSizeAttr(xml_attribute *, const char */* rhs */, size_t /* size */);
bool setValueAttr(xml_attribute *, const char */* rhs */);
bool setValueWithSizeAttr(xml_attribute *, const char */* rhs */, size_t /* size */);

bool setValueInt(xml_attribute *, int /* rhs */);
bool setValueUint(xml_attribute *, unsigned int /* rhs */);
bool setValueLong(xml_attribute *, long /* rhs */);
bool setValueULong(xml_attribute *, unsigned long /* rhs */);
bool setValueDouble(xml_attribute *, double /* rhs */);
bool setValueDoubleP(xml_attribute *, double /* rhs */, int /* precision */);
bool setValueFloat(xml_attribute *, float /* rhs */);
bool setValueFloatP(xml_attribute *, float /* rhs */, int /* precision */);
bool setValueBool(xml_attribute *, bool /* rhs */);

xml_attribute nextAttribute(const xml_attribute *);
xml_attribute previousAttribute(const xml_attribute *);

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
bool validNode(const xml_node *);
xml_node_type type(const xml_node *);
const char *getNameNode(const xml_node *);
const char *getValueNode(const xml_node *);

xml_attribute firstAttribute(const xml_node *);
xml_attribute lastAttribute(const xml_node *);
xml_node getFirstChild(const xml_node *);
xml_node getLasChild(const xml_node *);
xml_node nextSibling(const xml_node *);
xml_node previousSibling(const xml_node *);
xml_node getParentNode(const xml_node *);
xml_node getRootNode(const xml_node *);
struct xml_text getTextNode(const xml_node *);

xml_node getChildNode(const xml_node *, const char */* name */);
xml_attribute getAttributeNode(const xml_node *, const char */* name */);
xml_node getNextSiblingNode(const xml_node *, const char */* name */);
xml_node getPreviousSiblingNode(const xml_node *, const char */* name */);
xml_attribute getAttributeNodeHint(const xml_node *, xml_attribute *hint);
const char *getChidValueNode(const xml_node *);
const char *getChileValueNodeWithName(const xml_node *, const char */* name */);

bool setNameNode(xml_node *, const char */* rhs */);
bool setNameNodeWithSize(xml_node *, const char */* rhs */, size_t /* size */);
bool setValueNode(xml_node *, const char */* rhs */);
bool setValueNodeWithSize(xml_node *, const char */* rhs */, size_t /* size */);

xml_attribute appendAttributeNode(xml_node *, const char */* name */);
xml_attribute prependAttributeNode(xml_node *, const char */* name */);
xml_attribute insertAttributeAfterNode(xml_node *, const char */* name */, const xml_attribute */* attr */);
xml_attribute insertAttributeBeforeNode(xml_node *, const char */* name */, const xml_attribute */* attr */);

xml_attribute appendCopyNode(xml_node *, const xml_attribute */* proto */);
xml_attribute prependCopyNode(xml_node *, const xml_attribute */* proto */);
xml_attribute insertCopyAfterNode(xml_node *, const xml_attribute */* proto */, const xml_attribute */* attr */);
xml_attribute insertCopyBeforeNode(xml_node *, const xml_attribute */* proto */, const xml_attribute */* attr */);

xml_node appendChildNode(xml_node *, xml_node_type /* type */);
xml_node prependChildNode(xml_node *, xml_node_type /* type */);
xml_node insertChildAfterNode(xml_node *, xml_node_type /* type */, const xml_node */* node */);
xml_node insertChildBeforeNode(xml_node *, xml_node_type /* type */, const xml_node */* node */);

xml_node appendChildNameNode(xml_node *, const char */* name */);
xml_node prependChildNameNode(xml_node *, const char */* name */);
xml_node insertChildNameAfterNode(xml_node *, const char */* name */, const xml_node */* node */);
xml_node insertChildNameBeforeNode(xml_node *, const char */* name */, const xml_node */* node */);

xml_node appendCopyProtoNode(xml_node *, const xml_node */* proto */);
xml_node prependCopyProtoNode(xml_node *, const xml_node */* proto */);
xml_node insertCopyProtoAfterNode(xml_node *, const xml_node */* proto */, const xml_node */* node */);
xml_node insertCopyProtoBeforeNode(xml_node *, const xml_node */* proto */, const xml_node */* node */);

xml_node appendMoveNode(xml_node *, const xml_node */* moved */);
xml_node prependMoveNode(xml_node *, const xml_node */* moved */);
xml_node insertMoveAfterNode(xml_node *, const xml_node */* moved */, const xml_node */* node */);
xml_node insertMoveBeforeNode(xml_node *, const xml_node */* moved */, const xml_node */* node */);

bool removeAttributeNode(xml_node *, const xml_attribute */* attr */);
bool removeAttributeNameNode(xml_node *, const char */* name */);
bool removeAttributesNode(xml_node *);
bool removeChildNode(xml_node *, const xml_node */* node */);
bool removeChildNameNode(xml_node *, const char */* name */);
bool removeAllChildren(xml_node *);

struct xml_parse_result append_buffer(xml_node *, const void */* contents */, size_t /* size */, unsigned int /* options */, xml_encoding /* encoding */);

typedef bool (*PredicateAttr)(xml_attribute attr);

inline xml_attribute findAttributeNode(const xml_node *n, PredicateAttr pred) {
  if (!n->root) {
    return (xml_attribute){};
  }

  // IDK abt this...
  for (xml_attribute attr = firstAttribute(n); validAttr(&attr); attr = nextAttribute(&attr)) {
    if (pred(attr)) {
      return attr;
    }
  }

  return (xml_attribute){};
}

typedef bool (*PredicateNode)(xml_node node);

inline xml_node findChildNode(const xml_node *n, PredicateNode pred) {
  if (!n->root) return (xml_node){};

  for (xml_node node = getFirstChild(n); validNode(&node); node = nextSibling(&node)) {
    if (pred(node))
      return node;
  }

  return (xml_node){};
}

inline xml_node findNode(const xml_node *n, PredicateNode pred) {
  if (!n->root) return (xml_node){};

  xml_node cur = getFirstChild(n);

  while (cur.root && cur.root != n->root) {
    if (pred(cur)) return cur;

    xml_node firstChildNode = getFirstChild(&cur);
    xml_node nextSiblingNode = nextSibling(&cur);
    if (validNode(&firstChildNode)) {
      cur = firstChildNode;
    } else if (validNode(&nextSiblingNode)) {
      cur = nextSiblingNode;
    } else {
      while (!validNode(&nextSiblingNode) && cur.root != n->root) {
        cur = getParentNode(&cur);
      }

      if (cur.root != n->root) {
        cur = nextSibling(&cur);
      }
    }
  }

  return (xml_node){};
}

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
