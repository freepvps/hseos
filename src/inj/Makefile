all: main

run: main
	./main

main: main.c
	gcc main.c -o main -O3

libtemp.so: temp.c
	gcc temp.c -o libtemp.so -O3 --shared -ldl

clean:
	rm ./main
