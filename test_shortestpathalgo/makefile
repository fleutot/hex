CC = g++
CFLAGS = -Wall -Werror -O3

SRC = ../shortestpathalgo.cpp ../graph.cpp ../prioqueue.cpp shortestpathalgo_test.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = shortestpathalgo_test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) ../*.o *.o $(TARGET)

test: $(TARGET)
	./$(TARGET)
