target := hw0201 hw0202 hw0203 hw0204 hw0205

SRC := $(wildcard *.c)
DEP := $(SRC:.c=.d)
OBJ := $(SRC:.c=.o)

CC = -gcc
CFLAGS = -Wall -Wextra -Werror -MMD -MP

LDFLAGS = -Wall -Wextra -Werror
LDLIBS  = -lm

.PHONY: all clean

all: $(target)

debug: CFLAGS += -DDEBUG -g
debug: $(target)

hw0301: hw0201.o myIrrational.o
hw0302: hw0202.o
hw0303: hw0203.o
hw0304: hw0204.o
hw0305: hw0205.o

-include $(DEP)

clean:
	rm -f $(OBJ) $(DEP) $(target)
