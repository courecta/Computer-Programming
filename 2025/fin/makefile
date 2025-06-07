all:
	gcc fin01.c -o fin01 -lcurl || true
	gcc fin02.c -o fin02 -lcurl -ljson-c -lm || true
	gcc schedule.c -c -o schedule.o || true
	gcc schedule.o fin03.c -o fin03 || true

clean:
	rm -f fin01 fin02 fin03 schedule.o || true