build:
	gcc -Wall ./src/*.c `sdl2-config --libs --cflags` -lm -o game

run:
	./game

clean:
	rm game
