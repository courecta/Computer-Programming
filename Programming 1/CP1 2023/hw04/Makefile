all:
	gcc hw0401.c -c -o hw0401.o
	gcc mysort.c -c -o mysort.o
	gcc hw0401.o mysort.o -o hw0401

	gcc hw0402.c -c -o hw0402.o
	gcc derive.c -c -o derive.o
	gcc hw0402.o derive.o -o hw0402 -lm

	gcc hw0403.c -o hw0403 -lm

clean:
	rm -rf hw0401.o
	rm -rf mysort.o
	rm -rf hw0401

	rm -rf hw0402.o
	rm -rf derive.o
	rm -rf hw0402
	
	rm -rf hw0403