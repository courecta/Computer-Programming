target := hw0201 hw0202 hw0203 hw0204 hw0205

SRC := $(wildcard *.c)
DEP := $(SRC:.c=.d)
OBJ := $(SRC:.c=.o)

CC = -gcc
CFLAGS = -Wall -Wextra -MMD -MP -g

LDFLAGS = -Wall -Wextra -Werror
LDLIBS  = -lm

.PHONY: all clean

all: $(target)

debug: CFLAGS += -DDEBUG -g
debug: $(target)

hw0201: hw0201.o
hw0202: hw0202.o
hw0203: hw0203.o
hw0204: hw0204.o
hw0205: hw0205.o

-include $(DEP)

clean:
	rm -f $(OBJ) $(DEP) $(target)
