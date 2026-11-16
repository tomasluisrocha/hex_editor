#ifndef FILE_H
#define FILE_H

#include <stddef.h>

// Allocates memory for the whole file, sets file_size accordingly, must be freed with free() by the
// caller; Empty files get assigned a valid pointer
// Returns the pointer to the buffer on success, or NULL on failure
unsigned char *file_load(const char *filepath, size_t *file_size);

#endif