#ifndef EDITOR_H
#define EDITOR_H

#include <stddef.h>

#define ROW_SIZE 16

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
    size_t file_size;
    enum editor_mode mode;
    enum display_mode display;
};

void editor_move_left(struct editor *editor);
void editor_move_right(struct editor *editor);
void editor_move_up(struct editor *editor);
void editor_move_down(struct editor *editor);

#endif