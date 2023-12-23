all:
	gcc fin01.c -c -o fin01.o
	gcc frac.c -c -o frac.o
	gcc fin01.o frac.o -o fin01 -lm

	gcc fin02.c -c -o fin02.o
	gcc -g mine.c -c -o mine.o
	gcc fin02.o mine.o -o fin02
	
	gcc fin04.c -c -o fin04.o
	gcc game.c -c -o game.o
	gcc fin04.o game.o -o fin04 -lm

clean:
	rm -rf fin01.o
	rm -rf frac.o
	rm -rf fin01
	
	rm -rf fin02.o
	rm -rf mine.o
	rm -rf fin02
	
	rm -rf fin04.o
	rm -rf game.o
	rm -rf fin04