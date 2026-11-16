#include "terminal.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

static struct termios original_settings;
static bool raw_mode = false;
static bool atexit_registered = false;

void terminal_clear_screen(void) {
    printf("\033[2J");
    printf("\033[H");
}

void terminal_disable_raw_mode(void) {
    if (!raw_mode) {
        return;
    }

    // Put the terminal back exactly as we found it.
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_settings) == -1) {
        perror("tcsetattr (restore)");
    }

    raw_mode = false;
}

int terminal_enable_raw_mode(void) {
    if (raw_mode) {
        return 0;
    }

    if (tcgetattr(STDIN_FILENO, &original_settings) == -1) {
        perror("tcgetattr");
        return -1;
    }

    // From here on, any exit() or return from main() restores the terminal.
    if (!atexit_registered) {
        if (atexit(terminal_disable_raw_mode) == 0) {
            atexit_registered = true;
        } else {
            fprintf(stderr, "atexit(): could not register exit function\n");
            return -1;
        }
    }

    // Work on a copy; original_settings stays untouched for the restore.
    struct termios raw = original_settings;

    // Local flags
    raw.c_lflag &= ~ECHO;    // don't print typed keys; we draw the screen ourselves
    raw.c_lflag &= ~ICANON;  // deliver bytes immediately, not line by line
    raw.c_lflag &= ~ISIG;    // Ctrl-C / Ctrl-Z arrive as bytes instead of signals
    raw.c_lflag &= ~IEXTEN;  // Ctrl-V arrives as a byte instead of "quote next key"

    // Input flags
    raw.c_iflag &= ~IXON;   // Ctrl-S / Ctrl-Q arrive as bytes instead of freezing output
    raw.c_iflag &= ~ICRNL;  // Enter arrives as 13 (\r), not translated to 10 (\n)

    // read() blocks until at least 1 byte is available, with no timeout.
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        perror("tcsetattr");
        return -1;
    }

    raw_mode = true;
    return 0;
}

int terminal_read_key(void) {
    while (1) {
        unsigned char c;
        ssize_t n = read(STDIN_FILENO, &c, 1);

        if (n == -1) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }

        if (n == 0) {
            errno = EIO;
            return -1;
        }

        return (int)c;
    }
}
