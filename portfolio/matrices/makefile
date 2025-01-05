# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -lm

# Find all .c files in the current directory
SRCS = $(wildcard *.c)

# The name of the output executable
EXEC = main

# Default target
all: $(EXEC)

# Rule to create the executable
$(EXEC): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean up object files and the executable
clean:
	rm -f $(EXEC)