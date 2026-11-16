
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "file.h"

unsigned char *load_file(const char *filepath, size_t *file_size) {
    if (file_size == NULL) {
        errno = EINVAL;
        perror("load_file");
        return NULL;
    }

    *file_size = 0;

    int fd = open(filepath, O_RDONLY);

    if (fd == -1) {
        perror("open");
        return NULL;
    }

    unsigned char *dest = NULL;

    struct stat file_info;

    if (fstat(fd, &file_info) == -1) {
        perror("fstat");
        goto cleanup;
    }

    if (file_info.st_size < 0 || (uintmax_t)file_info.st_size > SIZE_MAX) {
        errno = EOVERFLOW;
        perror("file size");
        goto cleanup;
    }

    size_t expected_size = (size_t)file_info.st_size;

    // Allocate at least one byte, even for an empty file.
    dest = malloc(expected_size > 0 ? expected_size : 1);

    if (dest == NULL) {
        perror("malloc");
        goto cleanup;
    }

    size_t total_read = 0;

    while (total_read < expected_size) {
        ssize_t bytes_read = read(fd, dest + total_read, expected_size - total_read);

        if (bytes_read == -1) {
            if (errno == EINTR) {
                continue;
            }

            perror("read");
            goto cleanup;
        }

        if (bytes_read == 0) {
            errno = EIO;
            perror("read");
            goto cleanup;
        }

        total_read += (size_t)bytes_read;
    }

    if (close(fd) == -1) {
        perror("close");
        free(dest);
        return NULL;
    }

    *file_size = total_read;
    return dest;

cleanup:
    free(dest);
    close(fd);
    return NULL;
}
