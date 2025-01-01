OTHERS = ./src/utils/*.c ./src/game/*.c ./src/game/entities/*.c

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
	gcc ./tests/test_main/*.c $(OTHERS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o exec/game

run:
	./exec/game

clean:
	rm exec/*
