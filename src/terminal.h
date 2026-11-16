#ifndef TERMINAL_H
#define TERMINAL_H

// Clears the screen and moves cursor home (top left)
void terminal_clear_screen(void);

// Restores the terminal to the user's previous state
// Safe to call at any time, even with raw mode already disabled, automatically called with atexit
// at exit() or return from main()
void terminal_disable_raw_mode(void);

// Enables raw mode if not already set, atexit is set to terminal_disable_raw_mode so terminal is
// restored from exit() or return from main()
// Returns 0 on success or if already set, or -1 on failure, along with an error message to stderr
int terminal_enable_raw_mode(void);

// Reads a single byte from standard input, blocks until a byte is received, no time limit
// Returns one byte, as an int from 0 to 255, or -1 on error (with errno set accordingly)
int terminal_read_key(void);

#endif