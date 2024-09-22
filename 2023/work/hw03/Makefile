all:
	gcc hw0301.c -c -o hw0301.o
	gcc mycircle.c -c -o mycircle.o
	gcc hw0301.o mycircle.o -o hw0301 -lm

	gcc hw0302.c -c -o hw0302.o
	gcc mycontrol.c -c -o mycontrol.o
	gcc hw0302.o mycontrol.o -o hw0302 -lm

	gcc hw0303.c -o hw0303
	
	gcc hw0304-1.c -c -o hw0304-1.o
	gcc hw0304.c hw0304-1.o -o hw0304-1 -lm
	gcc hw0304-2.c -c -o hw0304-2.o
	gcc hw0304.c hw0304-2.o -o hw0304-2 -lm
	
	gcc diceRolls.c -c -o diceRolls.o
	gcc hw0305.c -c -o hw0305.o
	gcc diceRolls.o hw0305.o -o hw0305
	

clean:
	rm -rf hw0301
	rm -rf mycircle.o
	rm -rf hw0301.o

	rm -rf hw0302
	rm -rf mycontrol.o
	rm -rf hw0302.o

	rm -rf hw0303
	
	rm -rf hw0304-1.o
	rm -rf hw0304-2.o
	rm -rf hw0304-1
	rm -rf hw0304-2
	
	rm -rf diceRolls.o
	rm -rf hw0305.o
	rm -rf hw0305
