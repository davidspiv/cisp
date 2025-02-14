CC=g++
CFLAGS= -c -g -Wall -std=c++17 -fpermissive
EXENAME= M02

default: main.o RSA.o
	$(CC) main.o RSA.o -o $(EXENAME)

main.o: main.cpp RSA.h io.h
	$(CC) $(CFLAGS) main.cpp -o main.o

RSA.o: RSA.cpp
	$(CC) $(CFLAGS) RSA.cpp -o RSA.o

clean:
	rm *.o $(EXENAME)

run:
	./$(EXENAME)
