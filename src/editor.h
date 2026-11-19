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
    int last_key;
};

// Moves the cursor in editor to the left (does nothing if already on the start of row)
void editor_move_left(struct editor *editor);

// Moves the cursor in editor to the right (does nothing if already on end of row or end of file)
void editor_move_right(struct editor *editor);

// Moves the cursor in editor up (does nothing if already on first line)
void editor_move_up(struct editor *editor);

// Moves the cursor in editor down (does nothing if already on last line)
void editor_move_down(struct editor *editor);

#endif