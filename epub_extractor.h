#ifndef __epub_extractor_h
#define __epub_extractor_h

#include "xml_scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TEMP_SIZE 512
#define BUF_SIZE 1024
#define MIMETYPE_CONTENTS "application/epub+zip"

struct s_epub {
  char *epub_filepath;
  char *extracted_dir;
  char *extracted_filepath;
  char *opf_filepath;
};
typedef struct s_epub Epub;

Epub *newEpub();
void closeEpub(Epub *epub);

void extractEpubFile(Epub *epub_ptr, char *file_path, char *result_filename);
char *getExtractDirectory(char *filepath);
char *extractEpubZip(char *filepath, char *dest_dir);
void confirmEpubFileExtension(char *filepath);
void confirmEpubFileType(char *contents);
char *findPackageContentLocation(Element *elements, size_t n_elements); // change parameter later
char *extractContentsFromFile(char *filename);
void str_cpyy(char *dest, char *src, size_t src_size);
char *getFileName(char *filepath, char *filename);

void processOPFFile(Epub *epub_ptr);

#endif
