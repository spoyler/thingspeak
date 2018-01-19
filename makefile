CC=g++
CFLAGS=-c -Wall -std=c++11

all: thingspeak 

thingspeak: main.o
	$(CC) main.o -o thingspeak.out -lboost_system -lpthread
	
mmain.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

clean:
	rm -rf *.o main
