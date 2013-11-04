CC = g++
CFLAGS = -Wall -Werror -O3

SRC = ../graph.cpp graph_test.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = graph_test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) ../*.o *.o $(TARGET)

test: $(TARGET)
	./$(TARGET)
