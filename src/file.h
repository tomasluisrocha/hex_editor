#ifndef FILE_H
#define FILE_H

#include <stddef.h>

unsigned char *file_load(const char *filepath, size_t *file_size);

#endif