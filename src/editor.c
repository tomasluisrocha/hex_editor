#include "editor.h"

void editor_move_left(struct editor *editor) {
    if (editor->cursor_offset > 0 && editor->cursor_offset % ROW_SIZE != 0) {
        editor->cursor_offset -= 1;
    }
}

void editor_move_right(struct editor *editor) {
    if (editor->cursor_offset + 1 < editor->file_size &&
        editor->cursor_offset % ROW_SIZE != ROW_SIZE - 1) {
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

void editor_scroll(struct editor *editor, size_t visible_rows) {
    if (visible_rows == 0 || editor->file_size == 0) {
        editor->first_row = 0;
        return;
    }
    size_t cursor_row = editor->cursor_offset / ROW_SIZE;
    if (cursor_row >= editor->first_row + visible_rows) {
        editor->first_row = (cursor_row - visible_rows + 1);
    }
    if (cursor_row < editor->first_row) {
        editor->first_row = (cursor_row);
    }
}
