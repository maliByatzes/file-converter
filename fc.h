#ifndef __fc_h
#define __fc_h

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// constants
#define TEMP_SIZE 512
#define BUF_SIZE 1024

// accepted file extensions
#define EPUB_EXT "epub"
#define MIMETYPE_FILE_NAME "/mimetype"
#define MIMETYPE_FILE_CONTENTS "application/epub+zip"

int confirmFileExtension(char *filepath, const char *expected_ext);
char *getFileName(char *filepath);
void extractEPUBFile(char *filepath, char *dir_path);
int confirmEPUBFileType(char *dir_path);
char *parseXMLFile(char *filepath);
char *getLocationOfPackageDocument(char *dir_path);

#endif
