all: main echo

main: hw.S
	gcc-7 hw.S -m32 -o ./main -g -lm -nostdlib

echo: echo.S
	gcc-7 echo.S -m32 -o ./echo -g -lm -nostdlib

clean:
	rm main
