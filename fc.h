#ifndef __fc_h
#define __fc_h

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// new API
void convertEpubToPDF(char *file_path, char *result_filename);
void convertPDFToEpub(char *file_path, char *result_filename);

/*
int confirmFileExtension(char *filepath, const char *expected_ext);
char *getFileName(char *filepath);
void extractEPUBFile(char *filepath, char *dir_path);
int confirmEPUBFileType(char *dir_path);
char *parseXMLFile(char *filepath);
char *getLocationOfPackageDocument(char *dir_path);
*/

#endif
