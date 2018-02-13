all: bin/main

bin/main: build/main.o build/memory.o
	gcc -Wall build/main.o build/memory.o -o bin/main

build/main.o: src/main.c
	gcc -std=c99 -Wall -I src/include src/main.c -c -o build/main.o

build/memory.o: src/memory.c
	gcc -std=c99 -Wall -I src/include src/memory.c -c -o build/memory.o

.PHONY:

clean:
	rm bin/*
	rm build/*

run:
	bin/main