#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "display.h"
#include "editor.h"
#include "file.h"
#include "terminal.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    struct editor editor = {
        .filename = argv[1],
        .cursor_offset = 0,
        .first_row = 0,
        .mode = MODE_NORMAL,
        .display = DISPLAY_HEX,
        .last_key = -1,
    };

    unsigned char *data = file_load(argv[1], &editor.file_size);

    if (data == NULL) {
        return 1;
    }

    if (terminal_open() == -1) {
        free(data);
        return 1;
    }

    bool running = true;
    while (running) {
        size_t rows = 0, columns = 0;
        if (terminal_get_size(&rows, &columns) == -1) {
            int saved_errno = errno;
            terminal_close();
            fprintf(stderr, "terminal_get_size: %s\n", strerror(saved_errno));
            free(data);
            return 1;
        }

        size_t visible_rows = (rows > RESERVED_ROWS) ? rows - RESERVED_ROWS : 0;

        editor_scroll(&editor, visible_rows);

        display_file(data, &editor, visible_rows, columns);

        int chr = terminal_read_key();
        if (chr == -1) {
            int saved_errno = errno;
            terminal_close();
            fprintf(stderr, "terminal_read_key: %s\n", strerror(saved_errno));
            free(data);
            return 1;
        }

        editor.last_key = chr;

        switch (chr) {
            case 'h':
                editor_move_left(&editor);
                break;
            case 'j':
                editor_move_down(&editor);
                break;
            case 'k':
                editor_move_up(&editor);
                break;
            case 'l':
                editor_move_right(&editor);
                break;
            case '0':
                editor_move_line_start(&editor);
                break;
            case '$':
                editor_move_line_end(&editor);
                break;
            case 'g':
                editor_move_line_first(&editor);
                break;
            case 'G':
                editor_move_line_last(&editor);
                break;
            case 6:  // Ctrl+F
                editor_move_page_down(&editor, visible_rows);
                break;
            case 2:  // Ctrl+B
                editor_move_page_up(&editor, visible_rows);
                break;
            case 4:  // Ctrl+D
                editor_move_half_page_down(&editor, visible_rows);
                break;
            case 21:  // Ctrl+U
                editor_move_half_page_up(&editor, visible_rows);
                break;
            case 'H':
                editor_move_screen_top(&editor);
                break;
            case 'M':
                editor_move_screen_middle(&editor, visible_rows);
                break;
            case 'L':
                editor_move_screen_bottom(&editor, visible_rows);
                break;
            case 'q':
                running = false;
                break;
            default:
                break;
        }
    }

    free(data);
    terminal_close();
    return 0;
}
