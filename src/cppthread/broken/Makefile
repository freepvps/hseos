all: main

run: main
	./main

main: main.cpp
	g++ main.cpp -o main -O1 -pedantic -Wall -lpthread --std=c++1z

clean:
	rm ./main
