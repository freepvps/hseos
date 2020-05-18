all: http-server

http-server: *.c *.h picohttpparser/*.c picohttpparser/*.h
	gcc *.c picohttpparser/*.c -o http-server -O3 -pedantic -Wall

clean:
	rm ./http-server
