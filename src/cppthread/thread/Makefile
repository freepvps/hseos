all: main

run: main
	./main

main: main.cpp
	g++ main.cpp -o main -O3 -pedantic -Wall -lpthread --std=c++1z

clean:
	rm ./main
