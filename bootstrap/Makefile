CXX = g++
CXXFLAGS = -I. -I./Components -I./Systems -std=c++17
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC = main.cpp

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