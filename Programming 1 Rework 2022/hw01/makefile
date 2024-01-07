all:
	gcc emailChecker.c -c -o emailChecker.o
	gcc myMail.c -c -o myMail.o
	gcc emailChecker.o myMail.o -o email -lm

clean:
	rm -rf emailChecker.o
	rm -rf myMail.o
	rm -rf email