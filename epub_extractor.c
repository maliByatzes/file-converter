#include "epub_extractor.h"
#include "xml_parser.h"

// TODO: use return statements instead of `exit()` to
// prevent memory leak on heap allocated pointers.

Epub *newEpub() {
  Epub *epub_ptr;

  epub_ptr = (Epub *)malloc(sizeof(Epub));

  if (epub_ptr == NULL) {
    perror("malloc error");
    exit(EXIT_FAILURE);
  }

  return epub_ptr;
}

void closeEpub(Epub *epub) {
  free(epub->extracted_dir);
  free(epub->extracted_filepath);
  free(epub);
}

void extractEpubFile(Epub *epub_ptr, char *file_path, char *result_filename) {
  (void)result_filename;

  char buf[TEMP_SIZE];

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

  str_cpyy(buf, extracted_filepath, strlen(extracted_filepath));

  strncat(buf, "/mimetype", 10);
  char *mimetype_filecontents = extractContentsFromFile(buf);

  confirmEpubFileType(mimetype_filecontents);

  XMLParser *xml_parser = newXMLParser();

  printf("extracted_filepath: %s\n", extracted_filepath);
  str_cpyy(buf, extracted_filepath, strlen(extracted_filepath));

  strncat(buf, "/META-INF/container.xml", 24);

  char *container_filecontents = extractContentsFromFile(buf);

  xml_parser->file_contents = container_filecontents;
  parseContent(xml_parser);

  // char *opf_filepath = findPackageContentLocation(xml_parser->tokens);
  // epub_ptr->opf_filepath = opf_filepath;

  closeXMLParser(xml_parser);
  free(mimetype_filecontents);
}

char *getExtractDirectory(char *filepath) {
  char *filename = NULL;

  char *extract_dir = (char *)malloc(BUF_SIZE);

  char temp_buf[TEMP_SIZE];
  memset(temp_buf, 0, TEMP_SIZE);

  str_cpyy(temp_buf, filepath, strlen(filepath));

  filename = getFileName(temp_buf, filename);

  snprintf(extract_dir, BUF_SIZE, "%s-dir", filename);

  return extract_dir;
}

char *extractEpubZip(char *filepath, char *dest_dir) {
  char *extracted_filepath = (char *)malloc(BUF_SIZE);

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

char *findPackageContentLocation(char *tokens) {
  (void) tokens;
  return NULL;
}

char *extractContentsFromFile(char *filename) {
  FILE *fptr;
  fptr = fopen(filename, "r");

  if (fptr == NULL) {
    perror("fopen error");
    exit(EXIT_FAILURE);
  }

  char *buffer = (char *)malloc(BUF_SIZE);
  memset(buffer, 0, BUF_SIZE);

  char temp[TEMP_SIZE];
  memset(temp, 0, TEMP_SIZE);

  while ((fgets(temp, TEMP_SIZE, fptr)) != NULL) {
    strncat(buffer, temp, TEMP_SIZE + 1);
    memset(temp, 0, TEMP_SIZE);
  }

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

char *getFileName(char *filepath, char *filename) {
  char *token = strtok(filepath, "/");
  while (token) {
    filename = token;
    token = strtok(NULL, "/");
  }
  return filename;
}
