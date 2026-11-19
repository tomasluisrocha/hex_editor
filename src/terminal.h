#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>

int terminal_open(void);
void terminal_close(void);

void terminal_cursor_home(void);  // printf("\033[H");
void terminal_clear_below(void);  // printf("\033[J");
void terminal_flush(void);        // fflush(stdout);
void terminal_clear_line(void);
void terminal_highlight_start(void);
void terminal_highlight_stop(void);

// Reads a single byte from standard input, blocks until a byte is received, no time limit
// Returns one byte, as an int from 0 to 255, or -1 on error (with errno set accordingly)
int terminal_read_key(void);

int terminal_get_size(size_t *rows, size_t *columns);
#endif