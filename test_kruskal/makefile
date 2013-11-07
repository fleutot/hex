CC = g++
CFLAGS = -Wall -Werror -O3 -std=c++11

SRC = ../mst_kruskal.cpp ../graph.cpp kruskal_test.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = kruskal_test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) ../*.o *.o $(TARGET)

test: $(TARGET)
	./$(TARGET)
