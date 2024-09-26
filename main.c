#include "fc.h"

int main(int argc, char **argv) {
  char *file_path = NULL;
  char *result_filename = NULL;

  // TODO: define robust flags to run the program either way
  // epub -> pdf conversion
  // pdf -> epub conversion
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <file_path> <result_filename>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  file_path = argv[1];
  result_filename = argv[2];

  if (strlen(result_filename) < 3) {
    fprintf(stderr, "result filename is too short. At least 3 characters required.\n");
    exit(EXIT_FAILURE);
  }

  // epub -> pdf = default
  convertEpubToPDF(file_path, result_filename);
  // pdf -> epub
  // convertPDFToEpub(file_path, result_filename);

  exit(EXIT_SUCCESS);
}
