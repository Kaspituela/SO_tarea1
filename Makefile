CC=gcc
CFLAGS=-I.

tarea: tarea.c
	$(CC) -o tarea tarea.c $(CFLAGS)

run:
		./tarea
