#include "fc.h"
#include "epub_extractor.h"

void convertEpubToPDF(char *file_path, char *result_filename) {
  Epub *epub_ptr = newEpub();

  extractEpubFile(epub_ptr, file_path, result_filename);

  printf("epub_filepath: %s\n", epub_ptr->epub_filepath);
  printf("extracted_dir: %s\n", epub_ptr->extracted_dir);
  printf("extracted_filepath: %s\n", epub_ptr->extracted_filepath);
  printf("opf_filepath: %s\n", epub_ptr->opf_filepath);

  processOPFFile(epub_ptr);

  closeEpub(epub_ptr);
}

void convertPDFToEpub(char *file_path, char *result_filename) {
  (void)file_path;
  (void)result_filename;
}
