CXX=g++
CXXFLAGS=-c -Wall -O0 -std=c++11 -g
LDLIBS=-lboost_system -lboost_program_options -lpthread	

SOURCES=$(shell find . -name *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=thingspeak
all: $(SOURCES) $(EXECUTABLE)
debug: $(SOURCES) $(EXECUTABLE)


$(EXECUTABLE):$(OBJECTS)
	$(CXX) $(OBJECTS) -o $@.out $(LDLIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	find . -name *.o | xargs rm
