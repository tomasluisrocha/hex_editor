#include <errno.h>
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

    size_t file_size = 0;

    unsigned char *data = load_file(argv[1], &file_size);

    if (data == NULL) {
        return 1;
    }

    if (enable_raw_mode() != 0) {
        free(data);
        return 1;
    }

    while (1) {
        int chr = read_key();

        if (chr == -1) {
            int saved_errno = errno;
            disable_raw_mode();
            fprintf(stderr, "read_key: %s\n", strerror(saved_errno));
            free(data);
            return 1;
        }

        printf("%d\r\n", chr);

        if (chr == 'q') {
            break;
        }
    }

    free(data);
    return 0;
}
