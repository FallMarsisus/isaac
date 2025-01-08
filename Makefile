OTHERS = 

#CC specifies which compiler we're using
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -Wall -lm

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_ttf -LSDL2_image

#This is the target that compiles our executable

build:
	find ./src/. -name "*.c" -exec gcc $(COMPILER_FLAGS) $(LINKER_FLAGS) -o bins/game {} +
#find . -name "*.c" -exec gcc -o {output_file} {} +
build_test:
	gcc ./tests/test_main/*.c $(OTHERS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o bins/game

run:
	./bins/game

clean:
	rm bins/*
