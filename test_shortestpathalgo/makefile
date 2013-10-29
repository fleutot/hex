CC = g++
CFLAGS = -Wall -Werror -O3

SRC = ../shortestpathalgo.cpp ../graph.cpp ../prioqueue.c shortestpathalgo_test.cpp
OBJ = $(SRC:.c=.o)
TARGET = shortestpathalgo_test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

.c.o:
	$(CC) $(CFLAGS) -cpp $< -o $@

clean:
	$(RM) ../*.o *.o $(TARGET)

test: $(TARGET)
	./$(TARGET)
