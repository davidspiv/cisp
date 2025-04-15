CC=g++
CFLAGS= -c -g -Wall -std=c++17 -fpermissive
EXENAME= M04

default: main.o Matrix.o
	$(CC) main.o Matrix.o -o $(EXENAME)

main.o: main.cpp Matrix.h io.h
	$(CC) $(CFLAGS) main.cpp -o main.o

Matrix.o: Matrix.cpp
	$(CC) $(CFLAGS) Matrix.cpp -o Matrix.o

clean:
	rm *.o $(EXENAME)

run: default
	./$(EXENAME)
