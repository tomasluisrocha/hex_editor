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
    const char *filename;
    size_t cursor_offset;
    size_t file_size;
    size_t first_row;
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

// Moves the cursor to the first column of the current row
void editor_move_line_start(struct editor *editor);

// Moves the cursor to the last column of the current row (or end of file)
void editor_move_line_end(struct editor *editor);

// Moves the cursor to the first row of the file, preserving column
void editor_move_line_first(struct editor *editor);

// Moves the cursor to the last row of the file, preserving column
void editor_move_line_last(struct editor *editor);

// Moves the cursor up by one screen of rows
void editor_move_page_up(struct editor *editor, size_t visible_rows);

// Moves the cursor down by one screen of rows
void editor_move_page_down(struct editor *editor, size_t visible_rows);

// Moves the cursor up by half a screen of rows
void editor_move_half_page_up(struct editor *editor, size_t visible_rows);

// Moves the cursor down by half a screen of rows
void editor_move_half_page_down(struct editor *editor, size_t visible_rows);

// Moves the cursor to the top row of the current viewport, preserving column
void editor_move_screen_top(struct editor *editor);

// Moves the cursor to the middle row of the current viewport, preserving column
void editor_move_screen_middle(struct editor *editor, size_t visible_rows);

// Moves the cursor to the bottom row of the current viewport, preserving column
void editor_move_screen_bottom(struct editor *editor, size_t visible_rows);

// Adjusts first_row so cursor_offset remains within the visible viewport
void editor_scroll(struct editor *editor, size_t visible_rows);

#endif