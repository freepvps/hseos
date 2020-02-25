all: main

run: main
	./main

main: *.c
	gcc-7 -m32 *.c -o main -O3 -nostdlib

clean:
	rm ./main
