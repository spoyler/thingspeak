CXX=g++
CXXFLAGS=-c -Wall -O0 -std=c++11 -ggdb
LDLIBS=-lboost_system -lpthread	

all: thingspeak 
debug: thingspeak

thingspeak: main.o
	$(CXX) main.o -o thingspeak.out $(LDLIBS)

mmain.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp

clean:
	rm -rf *.o main
