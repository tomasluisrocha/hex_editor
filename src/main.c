#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <errno.h>

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

    // Get filesize
    struct stat file_info;

    if (fstat(fd, &file_info) == -1)
    {
        perror("fstat");
        close(fd);
        return 1;
    }

    // Allocate memory for the whole file
    unsigned char *data = malloc(file_info.st_size);

    if (data == NULL)
    {
        perror("malloc");
        close(fd);
        return 1;
    }

    // Copy file into data
    size_t total_read = 0;
    size_t file_size = (size_t)file_info.st_size;

    while (total_read < file_size)
    {
        ssize_t bytes_read = read(fd, data + total_read, file_size - total_read);

        if (bytes_read == -1)
        {
            if (errno == EINTR)
                continue;

            perror("read");
            close(fd);
            return 1;
        }
        if (bytes_read == 0)
        {
            break;
        }

        total_read += (size_t)bytes_read;
    }

    for (size_t offset = 0; offset < total_read; offset += 16)
    {
        printf("%08jx: ", offset);

        size_t remaining = total_read - offset;
        size_t row_size = remaining < 16 ? remaining : 16;

        // Hex values for each byte
        for (size_t i = 0; i < row_size; i++)
        {
            printf("%02X ", data[i + offset]);
        }

        // Padding for ASCII charactes
        for (int i = row_size; i < 16; i++)
        {
            printf("   ");
        }

        printf("| ");

        // ASCII representation
        for (size_t i = 0; i < row_size; i++)
        {
            if (isprint(data[i + offset]))
                printf("%c", data[i + offset]);
            else
                printf(".");
        }
        printf("\n");
    }

    close(fd);
    return 0;
}