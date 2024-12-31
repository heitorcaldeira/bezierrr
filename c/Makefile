CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -pedantic -ggdb `pkg-config --cflags sdl2`
LIBS    = `pkg-config --libs sdl2` -lm

bezier: bezier.c
	$(CC) $(CFLAGS) -o bezier bezier.c $(LIBS)
