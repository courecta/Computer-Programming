all:
	gcc hw0501.c -c -o hw0501.o
	gcc mystatistics.c -c -o mystatistics.o
	gcc hw0501.o mystatistics.o -o hw0501 -lm

	gcc hw0502.c -c -o hw0502.o
	gcc -g myge.c -c -o myge.o
	gcc hw0502.o myge.o -o hw0502 -lm

	gcc hw0503.c -c -o hw0503.o
	gcc mysphere.c -c -o mysphere.o
	gcc hw0503.o mysphere.o -o hw0503 -lm

	gcc hw0504.c -c -o hw0504.o
	gcc mytlv.c -c -o mytlv.o
	gcc hw0504.o mytlv.o -o hw0504

	gcc hw0505.c -c -o hw0505.o
	gcc -g tas.c -c -o tas.o
	gcc decode.c -c -o decode.o
	gcc hw0505.o tas.o decode.o -o hw0505

clean:
	rm -rf hw0501.o
	rm -rf mystatistics.o
	rm -rf hw0501

	rm -rf hw0502.o
	rm -rf myge.o
	rm -rf hw0502

	rm -rf hw0503.o
	rm -rf mysphere.o
	rm -rf hw0503

	rm -rf hw0504.o
	rm -rf mytlv.o
	rm -rf hw0504

	rm -rf hw0505.o
	rm -rf tas.o
	rm -rf decode.o
	rm -rf output.fm2
	rm -rf hw0505