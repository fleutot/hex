CC = g++
CFLAGS = -Wall -Werror -O3

SRC = graph.cpp prioqueue.cpp shortestpathalgo.cpp average_shortest_path.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = average_shortest_path

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

.c.o:
	$(CC) $(CFLAGS) -cpp $< -o $@

clean:
	$(RM) *.o $(TARGET)

run: $(TARGET)
	./$(TARGET)
