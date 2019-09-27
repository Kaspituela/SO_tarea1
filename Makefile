CC=gcc

run: tarea
	./main

tarea:
	$(CC) -o main tarea.c

rm:
	rm -rf main ./*/
