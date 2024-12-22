build:
	gcc -Wall ./src/*.c `sdl2-config --libs --cflags` -lm -o exec/game

run:
	./exec/game

test_entities:
	gcc  ./src/game/entities.c ./src/game/list_entities.c ./tests/game/test_entities.c -o exec/entity && ./exec/entity

test_rooms:
	gcc  ./src/utils/utils.c ./src/game/rooms.c ./src/game/list_entities.c ./tests/game/test_rooms.c -o exec/entity && ./exec/entity

test_chained_list:
	gcc ./src/utils/chained_list.c ./tests/utils/test_chained_list.c -o exec/chained_list && ./exec/chained_list

clean:
	rm exec/*
