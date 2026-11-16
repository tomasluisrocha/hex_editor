#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stddef.h>
#include <ctype.h>
#include "display.h"

#define ROW_SIZE 16

void display_file(const unsigned char *data, size_t file_size)
{
    for (size_t offset = 0; offset < file_size; offset += ROW_SIZE)
    {

        printf("%08jx: ", (uintmax_t)offset);

        size_t remaining = file_size - offset;
        size_t row_size = remaining < ROW_SIZE ? remaining : ROW_SIZE;

        // Hexadecimal representation
        for (size_t i = 0; i < row_size; i++)
        {
            printf("%02X ", data[offset + i]);
        }

        // Padding for incomplete rows
        for (size_t i = row_size; i < ROW_SIZE; i++)
        {
            printf("   ");
        }

        printf("| ");

        // ASCII representation
        for (size_t i = 0; i < row_size; i++)
        {
            unsigned char byte = data[offset + i];

            if (isprint(byte))
            {
                printf("%c", byte);
            }
            else
            {
                printf(".");
            }
        }

        printf("\n");
    }
}