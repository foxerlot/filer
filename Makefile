CC = gcc
CFLAGS = -O3 -Wall -Wextra -Werror -Wpedantic -std=c99
LFLAGS = -lncurses

SOURCES = $(wildcard src/*.c)
TARGET = filer

all: clean build

debug: CFLAGS = -g -v -Wall -Wextra -Werror -Wpedantic -std=c99
debug: clean build

build:
	$(CC) $(CFLAGS) $(SOURCES) $(LFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

uninstall: clean
	rm -f /usr/local/bin/$(TARGET)

.PHONY: all debug clean build install uninstall
