CC = g++
CFLAGS = -Wall -Werror -O3

SRC = ../graph.cpp graph_test.cpp
OBJ = $(SRC:.c=.o)
TARGET = graph_test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

.c.o:
	$(CC) $(CFLAGS) -cpp $< -o $@

clean:
	$(RM) ../*.o *.o $(TARGET)

test: $(TARGET)
	./$(TARGET)
