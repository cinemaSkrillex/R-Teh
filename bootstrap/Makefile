CXX = g++
CXXFLAGS = -I. -I./ -I./Systems -std=c++17 $(shell find include -type d -exec echo -I{} \;)
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC = main.cpp $(shell find src -name '*.cpp')

OBJ = $(SRC:.cpp=.o)

TARGET = a.out

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

re : clean all

.PHONY: all clean