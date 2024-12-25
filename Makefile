OTHERS = ./src/utils/*.c ./src/game/*.c

#CC specifies which compiler we're using
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -Wall -lm

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_ttf -LSDL2_image 

#This is the target that compiles our executable

build:
	gcc ./src/*.c $(OTHERS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o exec/game

build_test:
	gcc -Wall ./src/utils/*.c ./src/game/*.c ./tests/test_main.c `sdl2-config --libs --cflags` -lm -o exec/game

run:
	./exec/game

test_chained_list:
	gcc ./src/utils/chained_list.c ./tests/utils/test_chained_list.c -o exec/chained_list && ./exec/chained_list

clean:
	rm exec/*
