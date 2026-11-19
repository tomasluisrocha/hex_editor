#include "editor.h"

static size_t editor_row(size_t offset) {
    return offset / ROW_SIZE;
}

static size_t editor_col(size_t offset) {
    return offset % ROW_SIZE;
}

static size_t editor_last_row(size_t file_size) {
    return (file_size - 1) / ROW_SIZE;
}

// Saturating subtraction: returns a - b, clamping to 0 instead of underflowing
static size_t sub_sat(size_t a, size_t b) {
    return a > b ? a - b : 0;
}

static void editor_set_pos(struct editor *editor, size_t row, size_t column) {
    if (editor->file_size == 0) {
        editor->cursor_offset = 0;
        return;
    }

    size_t offset = row * ROW_SIZE + column;
    editor->cursor_offset = (offset < editor->file_size) ? offset : editor->file_size - 1;
}

static void editor_page_down_by(struct editor *editor, size_t visible_rows, size_t step) {
    if (editor->file_size == 0 || visible_rows == 0 || step == 0) {
        return;
    }

    size_t last = editor_last_row(editor->file_size);
    size_t max_first = sub_sat(last, visible_rows - 1);

    size_t new_first = editor->first_row + step;
    editor->first_row = new_first < max_first ? new_first : max_first;

    editor_set_pos(editor, editor_row(editor->cursor_offset) + step,
                   editor_col(editor->cursor_offset));
}

static void editor_page_up_by(struct editor *editor, size_t visible_rows, size_t step) {
    if (editor->file_size == 0 || visible_rows == 0 || step == 0) {
        return;
    }

    editor->first_row = sub_sat(editor->first_row, step);
    editor_set_pos(editor, sub_sat(editor_row(editor->cursor_offset), step),
                   editor_col(editor->cursor_offset));
}

void editor_move_left(struct editor *editor) {
    if (editor_col(editor->cursor_offset) > 0) {
        editor->cursor_offset--;
    }
}

void editor_move_right(struct editor *editor) {
    if (editor->cursor_offset + 1 < editor->file_size &&
        editor_col(editor->cursor_offset) + 1 < ROW_SIZE) {
        editor->cursor_offset++;
    }
}

void editor_move_up(struct editor *editor) {
    editor_set_pos(editor, sub_sat(editor_row(editor->cursor_offset), 1),
                   editor_col(editor->cursor_offset));
}

void editor_move_down(struct editor *editor) {
    if (editor->file_size == 0) {
        return;
    }
    size_t row = editor_row(editor->cursor_offset);
    if (row < editor_last_row(editor->file_size)) {
        editor_set_pos(editor, row + 1, editor_col(editor->cursor_offset));
    }
}

void editor_move_line_start(struct editor *editor) {
    editor_set_pos(editor, editor_row(editor->cursor_offset), 0);
}

void editor_move_line_end(struct editor *editor) {
    editor_set_pos(editor, editor_row(editor->cursor_offset), ROW_SIZE - 1);
}

void editor_move_line_first(struct editor *editor) {
    editor_set_pos(editor, 0, editor_col(editor->cursor_offset));
}

void editor_move_line_last(struct editor *editor) {
    if (editor->file_size == 0) {
        return;
    }
    editor_set_pos(editor, editor_last_row(editor->file_size), editor_col(editor->cursor_offset));
}

void editor_move_page_up(struct editor *editor, size_t visible_rows) {
    editor_page_up_by(editor, visible_rows, visible_rows);
}

void editor_move_page_down(struct editor *editor, size_t visible_rows) {
    editor_page_down_by(editor, visible_rows, visible_rows);
}

void editor_move_half_page_up(struct editor *editor, size_t visible_rows) {
    size_t step = visible_rows > 1 ? visible_rows / 2 : 1;
    editor_page_up_by(editor, visible_rows, step);
}

void editor_move_half_page_down(struct editor *editor, size_t visible_rows) {
    size_t step = visible_rows > 1 ? visible_rows / 2 : 1;
    editor_page_down_by(editor, visible_rows, step);
}

void editor_move_screen_top(struct editor *editor) {
    editor_set_pos(editor, editor->first_row, editor_col(editor->cursor_offset));
}

void editor_move_screen_middle(struct editor *editor, size_t visible_rows) {
    if (editor->file_size == 0) {
        return;
    }
    size_t last = editor_last_row(editor->file_size);
    size_t bottom = editor->first_row + sub_sat(visible_rows, 1);
    if (bottom > last) {
        bottom = last;
    }
    size_t mid = editor->first_row + (bottom - editor->first_row) / 2;
    editor_set_pos(editor, mid, editor_col(editor->cursor_offset));
}

void editor_move_screen_bottom(struct editor *editor, size_t visible_rows) {
    if (editor->file_size == 0) {
        return;
    }
    size_t last = editor_last_row(editor->file_size);
    size_t bottom = editor->first_row + sub_sat(visible_rows, 1);
    if (bottom > last) {
        bottom = last;
    }
    editor_set_pos(editor, bottom, editor_col(editor->cursor_offset));
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