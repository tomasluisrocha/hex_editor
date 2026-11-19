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

static void terminal_disable_raw_mode(void) {
    // Put the terminal back exactly as we found it.
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_settings) == -1) {
        perror("tcsetattr (restore)");
    }

    raw_mode = false;
}

static int terminal_enable_raw_mode(void) {
    if (raw_mode) {
        return 0;
    }

    if (tcgetattr(STDIN_FILENO, &original_settings) == -1) {
        perror("tcgetattr");
        return -1;
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

static void terminal_hide_cursor(void) {
    printf("\033[?25l");
}
static void terminal_show_cursor(void) {
    printf("\033[?25h");
}

int terminal_open(void) {
    // From here on, any exit() or return from main() restores the terminal.
    if (!atexit_registered) {
        if (atexit(terminal_close) == 0) {
            atexit_registered = true;
        } else {
            fprintf(stderr, "atexit(): could not register exit function\n");
            return -1;
        }
    }

    if (setvbuf(stdout, NULL, _IOFBF, 65536) != 0) {
        perror("setvbuf");
        return -1;
    }

    if (terminal_enable_raw_mode() == -1) {
        return -1;
    }

    printf("\033[?1049h");
    terminal_hide_cursor();
    fflush(stdout);
    return 0;
}

void terminal_close(void) {
    if (!raw_mode) {
        return;
    }
    terminal_show_cursor();
    printf("\033[?1049l");
    fflush(stdout);
    terminal_disable_raw_mode();
}

void terminal_cursor_home(void) {
    printf("\033[H");
}
void terminal_clear_below(void) {
    printf("\033[J");
}
void terminal_flush(void) {
    fflush(stdout);
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
