CXX	= g++
CXXFLAGS= -Wall -Wextra -std=c++11 -g

LIBS = -lGL -lGLU -lglut -lm

TARGET = trabalhocg


SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)