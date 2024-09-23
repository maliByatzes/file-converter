#ifndef __fc_h
#define __fc_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// constants
#define TEMP_SIZE 512

// accepted file extensions
#define EPUB_EXT "epub"

int confirmFileExtension(char *filepath, const char *expected_ext);

#endif
