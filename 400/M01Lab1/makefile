# I am a comment, and I want to say that the variable CC will be # the compiler to use.

CC=g++

# Hey!, I am comment number 2. I want to say that CFLAGS will be the # options I'll pass to the compiler.
CFLAGS= -c -g -Wall -std=c++17 -fpermissive
EXENAME= hello

default: main.o factorial.o hello.o
	$(CC) main.o factorial.o hello.o -o $(EXENAME)

main.o: main.cpp functions.h
	$(CC) $(CFLAGS) main.cpp

factorial.o: factorial.cpp functions.h
	$(CC) $(CFLAGS) factorial.cpp

hello.o: hello.cpp functions.h
	$(CC) $(CFLAGS) hello.cpp

clean:
	rm *.o $(EXENAME)

run:
	./$(EXENAME)
