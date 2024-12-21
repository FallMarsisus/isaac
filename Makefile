build:
	gcc -Wall ./src/*.c `sdl2-config --libs --cflags` -lm -o ./bins/game

run:
	./bins/game

clean:
	rm ./bins/game
