#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "display.h"
#include "editor.h"
#include "file.h"

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

    display_file(data, file_size, &editor);

    free(data);

    return 0;
}
