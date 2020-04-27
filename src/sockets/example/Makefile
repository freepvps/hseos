all: main server client

run: main
	./main

main: main.c types.h types.c
	gcc main.c types.c -o main -O3 -pedantic -Wall

server: server.c types.h types.c
	gcc server.c types.c -o server -O3 -pedantic -Wall

client: client.c types.h types.c
	gcc client.c types.c -o client -O3 -pedantic -Wall

clean:
	rm ./main
