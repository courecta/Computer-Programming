CC = gcc
CFLAGS = -I.
LIBS = -lm
DEPS = myMail.h
OBJ = emailChecker.o myMail.o
EXECUTABLE = email

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

email: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJ)
	rm -f $(EXECUTABLE)