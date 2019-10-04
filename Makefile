CC=gcc
FLAGS=-Wall
run: tarea
	./main

tarea:
	$(CC) -o main tarea.c $(FLAGS)

rm:
	rm -rf main ./*/
