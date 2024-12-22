build:
	gcc -Wall ./src/utils/*.c ./src/game/*.c ./src/*.c `sdl2-config --libs --cflags` -lm -o exec/game

run:
	./exec/game

test_chained_list:
	gcc ./src/utils/chained_list.c ./tests/utils/test_chained_list.c -o exec/chained_list && ./exec/chained_list

clean:
	rm exec/*
