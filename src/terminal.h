#ifndef TERMINAL_H
#define TERMINAL_H

// Clears the entire visible terminal screen and moves the cursor to the home position
void terminal_clear_screen(void);

// Prepares the terminal for a new frame by moving the cursor to the home position
void terminal_begin_frame(void);

// Finishes the current frame by clearing the remaining screen and flushing stdout
void terminal_end_frame(void);

// Restores the terminal's original settings; safe to call if raw mode is already disabled
// Automatically registered with atexit() by terminal_enable_raw_mode()
void terminal_disable_raw_mode(void);

// Enables raw mode; registers terminal_disable_raw_mode() with atexit() on first use
// Returns 0 on success or if already enabled, or -1 on failure
int terminal_enable_raw_mode(void);

// Reads a single byte from standard input, blocks until a byte is received, no time limit
// Returns one byte, as an int from 0 to 255, or -1 on error (with errno set accordingly)
int terminal_read_key(void);

#endif