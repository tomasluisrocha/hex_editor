#include "display.h"
#include "file.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    size_t file_size = 0;

    unsigned char *data = load_file(argv[1], &file_size);

    if (data == NULL) {
        return 1;
    }

    display_file(data, file_size);

    free(data);

    return 0;
}
