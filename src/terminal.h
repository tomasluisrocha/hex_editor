#ifndef TERMINAL_H
#define TERMINAL_H

void disable_raw_mode(void);
int enable_raw_mode(void);
int read_key(void);

#endif