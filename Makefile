CC=gcc
CFLAGS=-I. -g

tarea: tarea.c
	$(CC) -o tarea tarea.c $(CFLAGS)

run:
		./tarea
