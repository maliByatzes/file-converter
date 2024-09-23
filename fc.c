#include "fc.h"

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
