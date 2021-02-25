CC=gcc

build: all

all: ./src/process.c ./src/mlfq.c ./src/util.c
	gcc -g -o mlfq ./src/process.c ./src/mlfq.c ./src/util.c

clean:
	rm ./mlfq