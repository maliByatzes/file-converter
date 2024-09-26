#ifndef __fc_h
#define __fc_h

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void convertEpubToPDF(char *file_path, char *result_filename);
void convertPDFToEpub(char *file_path, char *result_filename);

#endif
