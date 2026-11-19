#ifndef DISPLAY_H
#define DISPLAY_H
#include <stddef.h>

#include "editor.h"

#define RESERVED_ROWS 2
#define MIN_COLS 76

// Prints the file stored in data, according to the settings set by editor
void display_file(const unsigned char *data, const struct editor *editor, size_t visible_rows,
                  size_t columns);

#endif