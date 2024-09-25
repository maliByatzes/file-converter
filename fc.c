#include "fc.h"

void convertEpubToPDF(char *file_path, char *result_filename) {}

void convertPDFToEpub(char *file_path, char *result_filename) {}
/*
int confirmFileExtension(char *filepath, const char *expected_ext) {
  char *filename = getFileName(filepath);

  char *actual_ext;
  char *token = strtok(filename, ".");
  while (token) {
    actual_ext = token;
    token = strtok(NULL, ".");
  }

  if ((strcmp(actual_ext, expected_ext)) != 0) {
    return -1;
  }

  return 0;
}

char *getFileName(char *filepath) {
  char *filename;

  char *token = strtok(filepath, "/");
  while (token) {
    filename = token;
    token = strtok(NULL, "/");
  }

  return filename;
}

void extractEPUBFile(char *filepath, char *dir_path) {
  char command[512];
  snprintf(command, sizeof(command), "unzip -q -d %s %s", dir_path, filepath);
  system(command);
}

int confirmEPUBFileType(char *dir_path) {
  strncat(dir_path, MIMETYPE_FILE_NAME, 10);
  FILE *fptr;
  fptr = fopen(dir_path, "r");
  if (fptr == NULL) {
    return -1;
  }

  char buffer[100];
  fgets(buffer, 100, fptr);

  if (ferror(fptr)) {
    fprintf(stderr, "error indicator set");
    fclose(fptr);
    return 1;
  }

  if ((strcmp(buffer, MIMETYPE_FILE_CONTENTS)) != 0) {
    fprintf(stderr, "error: invalid file type.\n");
    fclose(fptr);
    return 1;
  }

  fclose(fptr);
  return 0;
}

// open .xml file and return all its contents
char *parseXMLFile(char *filepath) {
  FILE *fptr;
  fptr = fopen(filepath, "r");
  if (fptr == NULL) {
    perror("fopen error");
    return NULL;
  }

  char *buffer = malloc(BUF_SIZE);
  memset(buffer, 0, BUF_SIZE);
  char temp[TEMP_SIZE];
  memset(&temp, 0, TEMP_SIZE);
  while ((fgets(temp, TEMP_SIZE, fptr)) != NULL) {
    strncat(buffer, temp, TEMP_SIZE + 1);
    memset(&temp, 0, TEMP_SIZE);
  };

  if (ferror(fptr)) {
    fprintf(stderr, "error indicator set");
    fclose(fptr);
    return NULL;
  }

  return buffer;
}

char *getLocationOfPackageDocument(char *dir_path) {
  char *xml_file_contents = parseXMLFile(dir_path);

  if (xml_file_contents == NULL) {
    return NULL;
  }

  char buf[9];
  for (char c = *xml_file_contents; c != '\0'; c = *++xml_file_contents) {
    strncat(buf, &c, 1);
  }

  // free(xml_file_contents);
  return NULL;
}*/
