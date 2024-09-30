#include "epub_extractor.h"

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
  free(epub->opf_filepath);
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

  XMLScanner *xml_parser = newXMLScanner();

  printf("extracted_filepath: %s\n", extracted_filepath);
  str_cpyy(buf, extracted_filepath, strlen(extracted_filepath));

  strncat(buf, "/META-INF/container.xml", 24);

  char *container_filecontents = extractContentsFromFile(buf);

  xml_parser->file_contents = container_filecontents;
  parseContent(xml_parser);

  char *opf_filepath = findPackageContentLocation(xml_parser->elements, xml_parser->n_elements);
  if (opf_filepath == NULL) {
    fprintf(stderr, "error: opf file path not found.\n");
    exit(EXIT_FAILURE);
  }

  printf("opf_filepath: %s\n", opf_filepath);
  epub_ptr->opf_filepath = opf_filepath;

  closeXMLScanner(xml_parser);
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

char *findPackageContentLocation(Element *elements, size_t n_elements) {
  // Loop through elements
  // Find the element->name == "rootfile"
  // Loop through the element->attributes
  // Find the attribute->name == "full-path"
  // Return the value of that "full-path" attribute

  char *opf_filepath = (char *)malloc(BUF_SIZE);
  memset(opf_filepath, 0, BUF_SIZE);

  for (size_t i = 0; i < n_elements; i++) {
    if ((strcmp(elements[i].name, "rootfile")) == 0) {

      for (size_t j = 0; j < elements[i].n_attributes; j++) {

        if ((strcmp(elements[i].attributes[j].name, "full-path")) == 0) {
          strncpy(opf_filepath, elements[i].attributes[j].value, BUF_SIZE);
          return opf_filepath;
        }
      }
    }
  }

  free(opf_filepath);
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
