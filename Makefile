CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -std=c17 -g
TARGET = hex_editor
SRC = src/main.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC) 
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) 

clean: 
	rm -f $(TARGET)


