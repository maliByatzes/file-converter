#include "epub_extractor.h"
#include "xml_parser.h"

// TODO: use return statements instead of `exit()` to
// prevent memory leak on heap allocated pointers.

Epub *extractEpubFile(char *file_path, char *result_filename) {
  Epub *epub_ptr;
  char buf[TEMP_SIZE];

  epub_ptr = malloc(sizeof(struct s_epub));

  if (epub_ptr == NULL) {
    perror("malloc error");
    exit(EXIT_FAILURE);
  }

  memset(epub_ptr, 0, sizeof(struct s_epub));
  epub_ptr->epub_filepath = file_path;

  str_cpyy(buf, file_path, strlen(file_path));
  confirmEpubFileExtension(buf);

  str_cpyy(buf, file_path, strlen(file_path));

  char *extract_dir = getExtractDirectory(buf);
  epub_ptr->extracted_dir = extract_dir;

  str_cpyy(buf, file_path, strlen(file_path));

  char *extracted_filepath = extractEpubZip(buf, extract_dir);
  epub_ptr->extracted_filepath = extracted_filepath;

  strncat(extracted_filepath, "/mimetype", 10);
  char *mimetype_filecontents = extractContentsFromFile(extracted_filepath);

  confirmEpubFileType(mimetype_filecontents);

  /*
  XMLParser *xml_parser;
  xml_parser = malloc(sizeof(struct s_xmlparser));

  if (xml_parser == NULL) {
    perror("malloc error");
    free(extract_dir);
    free(extracted_filepath);
    free(mimetype_filecontents);
    exit(EXIT_FAILURE);
  }*/

  // extracted-dir/META-INF/container.xml
  // char *extracted_f2 = cleanExtractedFilepath(extracted_filepath);
  // strncat(extracted_f2, "/META-INF/container.xml", 24);
  // xml_parser->parseFile(extracted_f2);

  // char *opf_filepath = findPackageContentLocation(xml_parser->tokens);
  // epub_ptr->opf_filepath = opf_filepath;

  // free(xml_parser);
  free(mimetype_filecontents);

  return epub_ptr;
}

char *getExtractDirectory(char *filepath) {
  char *filename = NULL;

  char *extract_dir = malloc(BUF_SIZE);

  char temp_buf[TEMP_SIZE];
  memset(temp_buf, 0, TEMP_SIZE);

  str_cpyy(temp_buf, filepath, strlen(filepath));

  filename = getFileName(temp_buf, filename);

  snprintf(extract_dir, BUF_SIZE, "%s-dir", filename);

  return extract_dir;
}

char *extractEpubZip(char *filepath, char *dest_dir) {
  char *extracted_filepath = malloc(BUF_SIZE);

  snprintf(extracted_filepath, BUF_SIZE, "./files/%s", dest_dir);

  char command[TEMP_SIZE];

  snprintf(command, sizeof(command), "unzip -q -d %s %s", extracted_filepath,
           filepath);

  system(command);

  return extracted_filepath;
}

void confirmEpubFileExtension(char *filepath) {
  char *actual_ext = NULL, *token = NULL, *filename = NULL;
  char temp_buf[TEMP_SIZE];
  const char *expected_ext = "epub";

  str_cpyy(temp_buf, filepath, strlen(filepath));

  filename = getFileName(temp_buf, filename);

  token = strtok(filename, ".");
  while (token) {
    actual_ext = token;
    token = strtok(NULL, ".");
  }

  if ((strcmp(actual_ext, expected_ext)) != 0) {
    fprintf(stderr, "error: unsupported file extension.\n");
    exit(EXIT_FAILURE);
  }
}

void confirmEpubFileType(char *contents) {
  if ((strcmp(contents, MIMETYPE_CONTENTS)) != 0) {
    fprintf(stderr, "error: invalid epub file.\n");
    exit(EXIT_FAILURE);
  }
}

char *findPackageContentLocation(char *tokens) {}

char *extractContentsFromFile(char *filename) {
  FILE *fptr;
  fptr = fopen(filename, "r");

  if (fptr == NULL) {
    perror("fopen error");
    exit(EXIT_FAILURE);
  }

  char *buffer = malloc(BUF_SIZE);
  memset(buffer, 0, BUF_SIZE);

  fgets(buffer, BUF_SIZE, fptr);

  if (ferror(fptr)) {
    fprintf(stderr, "error indicator set");
    fclose(fptr);
    exit(EXIT_FAILURE);
  }

  return buffer;
}

void str_cpyy(char *dest, char *src, size_t src_size) {
  memset(dest, 0, strlen(dest));
  strncpy(dest, src, src_size);
}

char *cleanExtractDir(char *extracted_dir) {}

char *getFileName(char *filepath, char *filename) {
  char *token = strtok(filepath, "/");
  while (token) {
    filename = token;
    token = strtok(NULL, "/");
  }
  return filename;
}
