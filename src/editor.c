#include "editor.h"

void editor_move_left(struct editor *editor) {
    if (editor->cursor_offset > 0) {
        editor->cursor_offset -= 1;
    }
}

void editor_move_right(struct editor *editor) {
    if (editor->cursor_offset + 1 < editor->file_size) {
        editor->cursor_offset += 1;
    }
}

void editor_move_up(struct editor *editor) {
    if (editor->cursor_offset >= ROW_SIZE) {
        editor->cursor_offset -= ROW_SIZE;
    }
}

void editor_move_down(struct editor *editor) {
    if (editor->cursor_offset + ROW_SIZE < editor->file_size) {
        editor->cursor_offset += ROW_SIZE;
    }
}
