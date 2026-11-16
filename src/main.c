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
        .cursor_offset = 0,
        .mode = MODE_NORMAL,
        .display = DISPLAY_HEX,
    };

    unsigned char *data = file_load(argv[1], &editor.file_size);

    if (data == NULL) {
        return 1;
    }

    if (terminal_enable_raw_mode() == -1) {
        free(data);
        return 1;
    }

    setvbuf(stdout, NULL, _IOFBF, 65536);

    bool running = true;
    while (running) {
        terminal_clear_screen();
        display_file(data, &editor);

        fflush(stdout);

        int chr = terminal_read_key();

        switch (chr) {
            case -1: {
                int saved_errno = errno;
                terminal_disable_raw_mode();
                fprintf(stderr, "terminal_read_key: %s\n", strerror(saved_errno));
                free(data);
                return 1;
            }
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
            case 'q':
                running = false;
                break;
            default:
                break;
        }
    }

    free(data);
    return 0;
}
