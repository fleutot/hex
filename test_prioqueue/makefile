CC = g++
CFLAGS = -Wall -Werror -O3

SRC = ../prioqueue.cpp prioqueue_test.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = prioqueue_test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

.c.o:
	$(CC) $(CFLAGS) -cpp $< -o $@

clean:
	$(RM) ../*.o *.o $(TARGET)

test: $(TARGET)
	./$(TARGET)
