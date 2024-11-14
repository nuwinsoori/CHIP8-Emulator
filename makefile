CFLAGS= -std=c17 -Wall

all:
	gcc main.c -o chip8 $(CFLAGS) `pkg-config sdl3 --cflags --libs`
