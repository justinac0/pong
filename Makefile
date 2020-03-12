CC=gcc
CFLAGS=-Wall -Wextra -std=c99
LDFLAGS=-lSDL2 -lSDL2_ttf

all: main.c
	$(CC) $(CFLAGS) main.c -o pong $(LDFLAGS)

run:
	./pong

.PHONY: clean
clean:
	rm pong
