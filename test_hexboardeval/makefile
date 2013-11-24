CC = g++
CFLAGS = -Wall -Werror -O3 -std=c++11 -ggdb

SRC = ../graph.cpp ../hexboard.cpp ../hexboardeval.cpp hexboardeval_test.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = hexboardeval_test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) ../*.o *.o $(TARGET)

test: $(TARGET)
	./$(TARGET)
