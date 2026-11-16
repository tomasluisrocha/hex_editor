CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -std=c17 -g
TARGET = hex_editor

SRC = src/main.c src/file.c src/display.c src/terminal.c
HEADERS = src/file.h src/display.h src/editor.h src/terminal.h

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) 

clean: 
	rm -f $(TARGET)


