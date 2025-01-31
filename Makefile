# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g -lm
LFLAGS = -lSDL2 -lSDL2_ttf -LSDL2_image

# Source files
#SRC = src/display.c src/main.c src/game/game.c src/game/ecs/ecs.c src/game/ecs/systems.c src/utils/chained_list.c src/utils/dict.c src/utils/dyn_arrays.c src/utils/id_array.c src/utils/timer.c src/utils/utils.c src/utils/vector.c
SRC = $(shell find src -type f -name '*.c')

# Object files directory
OBJDIR = obj

# Object files
OBJ = $(SRC:%.c=$(OBJDIR)/%.o)

# Executable name
EXEC = bins/game

# Default target
all: $(EXEC)

run:
	$(EXEC)

run_debug:
	valgrind --leak-check=full --show-leak-kinds=all $(EXEC)

# Linking the executable
$(EXEC): $(OBJ)
	$(CC) $(LFLAGS) $(CFLAGS) $(OBJ) -o $(EXEC)

# Compiling source files to object files
$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(LFLAGS) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJ) $(EXEC)

# Phony targets
.PHONY: all clean