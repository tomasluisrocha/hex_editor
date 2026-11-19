#include "display.h"

#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "terminal.h"

static const char *display_editor_mode_str(enum editor_mode mode) {
    switch (mode) {
        case MODE_NORMAL:
            return "NORMAL";
        case MODE_VISUAL:
            return "VISUAL";
        case MODE_INSERT:
            return "INSERT";
        default:
            return "UNKNOWN";
    }
}

static void display_status_bar(const struct editor *editor, size_t columns,
                               const unsigned char *data) {
    terminal_highlight_start();
    int len = 0;

    unsigned char cur_byte = editor->file_size > 0 ? data[editor->cursor_offset] : 0;
    len = printf("\"%s\"      %zuB      0x%zx      byte: 0x%02X (%u) '%c'", editor->filename,
                 editor->file_size, editor->cursor_offset, cur_byte, cur_byte,
                 isprint(cur_byte) ? cur_byte : '.');

    if (len < 0) {
        len = 0;
    }

    for (size_t i = (size_t)len; i < columns; i++) {
        putchar(' ');
    }

    terminal_highlight_stop();
    terminal_clear_line();
}

static void display_mode_bar(const struct editor *editor, size_t columns) {
    terminal_highlight_start();
    int len = 0;

    len = printf("-- %s --", display_editor_mode_str(editor->mode));

    if (len < 0) {
        len = 0;
    }
    for (size_t i = (size_t)len; i < columns; i++) {
        putchar(' ');
    }
    terminal_highlight_stop();
    terminal_clear_line();
}

void display_file(const unsigned char *data, const struct editor *editor, size_t visible_rows,
                  size_t columns) {
    terminal_cursor_home();
    if (visible_rows == 0 || columns < MIN_COLS) {
        printf("Terminal too small");
        terminal_clear_line();
        printf("\n");
        terminal_clear_below();
        terminal_flush();
        return;
    }

    size_t rows_printed = 0;

    for (size_t offset = editor->first_row * ROW_SIZE;
         offset < ((editor->first_row + visible_rows) * ROW_SIZE) && offset < editor->file_size;
         offset += ROW_SIZE) {
        printf("%08jx: ", (uintmax_t)offset);

        size_t remaining = editor->file_size - offset;
        size_t row_size = remaining < ROW_SIZE ? remaining : ROW_SIZE;

        // Hexadecimal representation
        for (size_t i = 0; i < row_size; i++) {
            bool selected = editor->cursor_offset == offset + i;

            if (selected) {
                terminal_highlight_start();
            }

            printf("%02X", data[offset + i]);

            if (selected) {
                terminal_highlight_stop();
            }

            printf(" ");
        }

        // Padding for incomplete rows
        for (size_t i = row_size; i < ROW_SIZE; i++) {
            printf("   ");
        }

        printf("| ");

        // ASCII representation
        for (size_t i = 0; i < row_size; i++) {
            unsigned char byte = data[offset + i];
            bool selected = editor->cursor_offset == offset + i;

            if (selected) {
                terminal_highlight_start();
            }

            printf("%c", isprint(byte) ? byte : '.');

            if (selected) {
                terminal_highlight_stop();
            }
        }

        terminal_clear_line();
        printf("\n");
        rows_printed++;
    }

    terminal_clear_below();
    for (size_t i = rows_printed; i < visible_rows; i++) {
        printf("\n");
    }

    display_status_bar(editor, columns, data);
    printf("\n");
    display_mode_bar(editor, columns);

    terminal_flush();
}
