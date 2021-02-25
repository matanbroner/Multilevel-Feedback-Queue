CC=gcc

build: all

all: ./src/process.c ./src/mlfq.c ./src/util.c
	gcc -o mlfq ./src/process.c ./src/mlfq.c ./src/util.c

clean:
	rm ./mlfq