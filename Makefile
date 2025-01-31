CC = gcc
CFLAGS = -Wall -ansi -pedantic -std=c90

all: eat 

eat: main.o game.o random.o terminal.o newSleep.o
	$(CC) $(CFLAGS) main.o game.o random.o terminal.o newSleep.o -o eat -lm

main.o: main.c game.h random.h terminal.h newSleep.h
	$(CC) $(CFLAGS) -c main.c

game.o: game.c game.h
	$(CC) $(CFLAGS) -c game.c

random.o: random.c random.h
	$(CC) $(CFLAGS) -c random.c

terminal.o: terminal.c terminal.h
	$(CC) $(CFLAGS) -c terminal.c

newSleep.o: newSleep.c newSleep.h
	$(CC) $(CFLAGS) -c newSleep.c

clean:
	rm -f eat *.o

