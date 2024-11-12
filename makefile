CFLAGS= -std=c17 -Wall -Wextra

all:
	gcc main.c -o chip8 $(CFLAGS) `pkg-config sdl3 --cflags --libs`
