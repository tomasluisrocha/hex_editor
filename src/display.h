#ifndef DISPLAY_H
#define DISPLAY_H
#include <stddef.h>

#include "editor.h"

// Prints the file stored in data, according to the settings set by editor
void display_file(const unsigned char *data, const struct editor *editor);

#endif