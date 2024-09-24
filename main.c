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

  // make sure the input file is an .epub file
  if ((confirmFileExtension(temp, EPUB_EXT)) < 0) {
    fprintf(stderr, "error: unsupported file extension.\n");
    exit(EXIT_FAILURE);
  }

  // exctract .epub
  memset(&temp, 0, sizeof(temp));
  strncpy(temp, filepath, TEMP_SIZE);
  char *filename = getFileName(temp);
  char dir_path[512];
  snprintf(dir_path, sizeof(dir_path), "./files/%s-dir", filename);

  extractEPUBFile(filepath, dir_path);

  // confirm the mimetype file contents
  int ret = confirmEPUBFileType(dir_path);
  if (ret < 0) {
    perror("confirm file type error");
    exit(EXIT_FAILURE);
  } else if (ret > 0) {
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
