all: libtest.so

libtest.so: test.c
	gcc -O3 -fPIC -DPIC -c test.c -nostartfiles
	gcc -fPIC -shared test.o -olibtest.so -nostartfiles

clean:
	rm -f test.o ./libtest.so
