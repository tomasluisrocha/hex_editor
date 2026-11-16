#ifndef EDITOR_H
#define EDITOR_H

#include <stddef.h>

enum editor_mode {
    MODE_NORMAL,
    MODE_VISUAL,
    MODE_INSERT
};
enum display_mode {
    DISPLAY_ASCII,
    DISPLAY_HEX
};

struct editor {
    size_t cursor_offset;
    enum editor_mode mode;
    enum display_mode display;
};

#endif