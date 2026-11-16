#ifndef DISPLAY_H
#define DISPLAY_H
#include <stddef.h>

#include "editor.h"
#define ROW_SIZE 16

void display_file(const unsigned char *data, size_t file_size, const struct editor *editor);

#endif