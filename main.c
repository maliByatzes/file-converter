#include "fc.h"

int main(int argc, char **argv) {
  char *filepath; // .epub file path
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
  memset(&temp, 0, TEMP_SIZE);
  strncpy(temp, dir_path, TEMP_SIZE);
  int ret = confirmEPUBFileType(temp);
  if (ret < 0) {
    perror("confirm file type error");
    exit(EXIT_FAILURE);
  } else if (ret > 0) {
    exit(EXIT_FAILURE);
  }

  memset(&temp, 0, TEMP_SIZE);
  strncpy(temp, dir_path, TEMP_SIZE);
  strncat(temp, "/META-INF/container.xml", 24);

  getLocationOfPackageDocument(temp);

  exit(EXIT_SUCCESS);
}
