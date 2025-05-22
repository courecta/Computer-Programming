all:
	gcc hw0401.c -o hw0401 -lm -g -gdwarf-2 -gz=none || true
	gcc hw0402.c -o hw0402 -lcurl -lm -g -gdwarf-2 -gz=none || true
	gcc hw0403.c -o hw0403 -llzma -g -gdwarf-2 -gz=none || true
	gcc hw0404.c -o hw0404 -lelf -g -gdwarf-2 -gz=none || true

clean:
	rm hw0401 || true
	rm hw0402 || true
	rm hw0403 || true
	rm hw0404 || true