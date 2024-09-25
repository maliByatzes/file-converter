#include "epub_extractor.h"

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
  // 6. find the `package.opf` file location and update struct

  return epub_ptr;
}

char *getExtractDirectory(char *filepath) {}

char *extractEpubZip(char *filepath, char *dest_dir) {}

void confirmEpubFileExtension(char *filepath) {}

void confirmEpubFileType(char *contents) {}

char *extractContentsFromFile(char *filename) {}

void str_cpyy(char *src, char *dest, size_t size) {
  memset(&dest, 0, sizeof(dest));
  strncpy(dest, src, size);
}
