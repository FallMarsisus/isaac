# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g
LFLAGS = -lm -lSDL2 -lSDL2_ttf

# Detect OS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	CFLAGS += -I/opt/homebrew/include
	LFLAGS += -L/opt/homebrew/lib -lSDL2_image
endif

# Source files
SRC = $(shell find src -type f -name '*.c')

# Object files directory
OBJDIR = obj

# Object files
OBJ = $(patsubst src/%.c,$(OBJDIR)/%.o,$(SRC))

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
$(OBJDIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(LFLAGS) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJ) $(EXEC)

# Phony targets
.PHONY: all clean