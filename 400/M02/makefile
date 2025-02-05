CC=g++
CFLAGS= -c -g -Wall -std=c++17 -fpermissive
EXENAME= main

default: build/main.o build/RSA.o
	$(CC) build/main.o build/RSA.o -o $(EXENAME)

#order-only-prerequisite for build dir
build/main.o: main.cpp RSA.h io.h | build
	$(CC) $(CFLAGS) main.cpp -o build/main.o

build/RSA.o: RSA.cpp | build
	$(CC) $(CFLAGS) RSA.cpp -o build/RSA.o

build:
	mkdir -p $@

clean:
	rm build/*.o $(EXENAME)

run:
	./$(EXENAME)
