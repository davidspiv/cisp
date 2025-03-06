CC=g++
CFLAGS= -c -g -Wall -std=c++17 -fpermissive
EXENAME= M04

default: main.o Matrices.o
	$(CC) main.o Matrices.o -o $(EXENAME)

main.o: main.cpp Matrices.h io.h
	$(CC) $(CFLAGS) main.cpp -o main.o

Matrices.o: Matrices.cpp
	$(CC) $(CFLAGS) Matrices.cpp -o Matrices.o

clean:
	rm *.o $(EXENAME)

run: default
	./$(EXENAME)
