CXX=g++
CXXFLAGS=-c -Wall -O0 -std=c++11 -g
LDLIBS=-lboost_system -lpthread	

SOURCES=main.cpp thingspeak_channel.cpp http_client.cpp
OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=thingspeak
all: $(SOURCES) $(EXECUTABLE)
debug: $(SOURCES) $(EXECUTABLE)


$(EXECUTABLE):$(OBJECTS)
	$(CXX) $(OBJECTS) -o $@.out $(LDLIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf *.o main
