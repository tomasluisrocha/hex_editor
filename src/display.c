#include "display.h"

#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

void display_file(const unsigned char *data, const struct editor *editor) {
    for (size_t offset = 0; offset < editor->file_size; offset += ROW_SIZE) {
        printf("%08jx: ", (uintmax_t)offset);

        size_t remaining = editor->file_size - offset;
        size_t row_size = remaining < ROW_SIZE ? remaining : ROW_SIZE;

        // Hexadecimal representation
        for (size_t i = 0; i < row_size; i++) {
            bool selected = editor->cursor_offset == offset + i;

            if (selected) {
                printf("\033[7m");
            }

            printf("%02X", data[offset + i]);

            if (selected) {
                printf("\033[0m");
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
                printf("\033[7m");
            }

            printf("%c", isprint(byte) ? byte : '.');

            if (selected) {
                printf("\033[0m");
            }
        }

        printf("\n");
    }
}
