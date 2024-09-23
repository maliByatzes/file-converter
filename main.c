#include "fc.h"

int main(int argc, char **argv) {
  char *filepath;
  char temp[TEMP_SIZE];

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filepath>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  filepath = argv[1];
  memset(&temp, 0, sizeof(temp));
  strncpy(temp, filepath, TEMP_SIZE);

  if ((confirmFileExtension(temp, EPUB_EXT)) < 0) {
    fprintf(stderr, "error: unsupported file extension.\n");
    exit(EXIT_FAILURE);
  }

  return 0;
}
