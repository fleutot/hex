CC = g++
CFLAGS = -Wall -Werror -O3 -std=c++11 -ggdb

SRC = ../graph.cpp ../hexboard.cpp ../moveeval.cpp moveeval_test.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = moveeval_test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) ../*.o *.o $(TARGET)

test: $(TARGET)
	./$(TARGET)
