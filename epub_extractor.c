#include "epub_extractor.h"
#include "xml_parser.h"

Epub *extractEpubFile(char *file_path, char *result_filename) {
  // Steps:
  // 1. confirm that `file_path` is valid for .epub file extension
  // 2. perform a `unzip` command extract the .epub file
  // 3. open and read the `mimetype` file
  // 4. confirm the contents of `mimetype` type as valid for
  //    .epub file
  // 5. parse the `container.xml` file to get back tokens
  // 6. find the `package.opf` file location and update struct
  // 7. finally return the `s_epub` struct

  Epub *epub_ptr;
  char buf[TEMP_SIZE];

  epub_ptr = malloc(sizeof(struct s_epub));

  if (epub_ptr == NULL) {
    perror("malloc error");
    exit(EXIT_FAILURE);
  }

  memset(epub_ptr, 0, sizeof(struct s_epub));
  epub_ptr->epub_filepath = file_path;

  str_cpyy(file_path, buf, TEMP_SIZE);
  confirmEpubFileExtension(buf);

  str_cpyy(file_path, buf, TEMP_SIZE);
  char *extract_dir = getExtractDirectory(buf);
  epub_ptr->extracted_dir = extract_dir;

  str_cpyy(file_path, buf, TEMP_SIZE);
  char *extracted_filepath = extractEpubZip(buf, extract_dir);
  epub_ptr->extracted_filepath = extracted_filepath;

  // extracted-dir/mimemtype => extracted_dir + "/mimetype"
  strncat(extract_dir, "/mimetype", 10);
  char *mimetype_filecontents = extractContentsFromFile(extract_dir);

  confirmEpubFileType(extract_dir);

  // 5. parse the `container.xml` file to get back tokens

  XMLParser *xml_parser;
  xml_parser = malloc(sizeof(struct s_xmlparser));

  if (xml_parser == NULL) {
    perror("malloc error");
    exit(EXIT_FAILURE);
  }

  // extracted-dir/META-INF/container.xml
  char *extract_dir = cleanExtractDir(extract_dir);
  strncat(extract_dir, "/META-INF/container.xml", 24);
  // xml_parser->parseFile(extract_dir);

  // 6. find the `package.opf` file location and update struct
  // char *opf_filepath = findPackageContentLocation(xml_parser->tokens);
  // epub_ptr->opf_filepath = opf_filepath;

  free(xml_parser);
  return epub_ptr;
}

char *getExtractDirectory(char *filepath) {}

char *extractEpubZip(char *filepath, char *dest_dir) {}

void confirmEpubFileExtension(char *filepath) {}

void confirmEpubFileType(char *contents) {}

char *findPackageContentLocation(char *tokens) {}

char *extractContentsFromFile(char *filename) {}

void str_cpyy(char *src, char *dest, size_t size) {
  memset(&dest, 0, sizeof(dest));
  strncpy(dest, src, size);
}

char *cleanExtractDir(char *extracted_dir) {}
