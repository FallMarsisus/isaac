.PHONY: build run clean winbuild

build:
	gcc -Wall -g ./src/*.c ./src/*.h `sdl2-config --libs --cflags` -lSDL2_ttf -lm -o ./bins/game

run:
	./bins/game

clean:
	rm ./bins/game

winbuild:
	x86_64-w64-mingw32-gcc -Wall ./src/*.c -I/usr/x86_64-w64-mingw32/include \
	-L/usr/x86_64-w64-mingw32/lib -lSDL2 -lSDL2_ttf -lm -o ./bins/exeGame -mconsole