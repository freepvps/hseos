all: main

run: main
	./main

main: main.c sigreader.h
	gcc main.c -o main -O3 -pedantic -Wall

clean:
	rm ./main
