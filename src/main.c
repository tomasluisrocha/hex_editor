#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <inttypes.h>

int main(int argc, char **argv)
{
    // Check for valid usage
    if (argc != 2)
    {
        printf("Usage: %s filename\n", argv[0]);
        return 1;
    }

    // Open file in read only mode
    int fd = open(argv[1], O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    unsigned char buffer[16];
    ssize_t bytes_read;
    off_t byte_offset = 0;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
    {
        printf("%08jx: ", (uintmax_t)byte_offset);

        // Hex values for each byte
        for (size_t i = 0; i < bytes_read; i++)
        {
            printf("%02X ", buffer[i]);
        }

        // Padding for ASCII charactes
        for (int i = bytes_read; i < 16; i++)
        {
            printf("   ");
        }

        printf("| ");

        // ASCII representation
        for (size_t i = 0; i < (size_t)bytes_read; i++)
        {
            if (isprint(buffer[i]))
                printf("%c", buffer[i]);
            else
                printf(".");
        }
        printf("\n");

        byte_offset += bytes_read;
    }

    close(fd);
    return 0;
}