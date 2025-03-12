CC=g++
CFLAGS=-c -g -Wall -std=c++17 -fpermissive
EXENAME=String

default: main.o string.o
	$(CC)  main.o string.o -o $(EXENAME)

main.o: main.cpp string.h
	$(CC) $(CFLAGS) main.cpp -o main.o

string.o: string.cpp string.h
	$(CC) $(CFLAGS) string.cpp -o string.o

build:
	mkdir -p $@

clean:
	rm *.o $(EXENAME)

run:
	./$(EXENAME)

