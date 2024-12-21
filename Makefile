build:
	gcc -Wall ./src/*.c ./src/*.h `sdl2-config --libs --cflags` -lm -o ./bins/game

run:
	./bins/game

clean:
	rm ./bins/game
