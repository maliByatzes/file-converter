#include "fc.h"

int confirmFileExtension(char *filepath, const char *expected_ext) {
  char *filename;

  char *token = strtok(filepath, "/");
  while (token) {
    filename = token;
    token = strtok(NULL, "/");
  }

  char *actual_ext;
  token = strtok(filename, ".");
  while (token) {
    actual_ext = token;
    token = strtok(NULL, ".");
  }

  if ((strcmp(actual_ext, expected_ext)) != 0) {
    return -1;
  }

  return 0;
}
