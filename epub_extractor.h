#ifndef __epub_extractor_h
#define __epub_extractor_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TEMP_SIZE 512

struct s_epub {
  char *epub_filepath;
  char *extracted_dir;
  char *extracted_filepath;
  char *opf_filepath;
};
typedef struct s_epub Epub;

Epub *extractEpubFile(char *file_path, char *result_filename);
char *getExtractDirectory(char *filepath);
char *extractEpubZip(char *filepath, char *dest_dir);
void confirmEpubFileExtension(char *filepath);
void confirmEpubFileType(char *contents);
char *findPackageContentLocation(char *tokens); // change parameter later
char *extractContentsFromFile(char *filename);
void str_cpyy(char *src, char *dest, size_t size);
char *cleanExtractDir(char *extracted_dir);

#endif

// Steps:
// 1. confirm that `file_path` is valid for .epub file extension
// 2. perform a `unzip` command extract the .epub file
// 3. open and read the `mimetype` file
// 4. confirm the contents of `mimetype` type as valid for
//    .epub file
// 5. parse the `container.xml` file to get back tokens
// 6. find the `package.opf` file location and update struct
// 7. finally return the `s_epub` struct
