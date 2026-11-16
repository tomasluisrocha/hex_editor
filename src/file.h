#ifndef FILE_H
#define FILE_H

#include <stddef.h>

unsigned char *load_file(const char *filepath, size_t *file_size);

#endif