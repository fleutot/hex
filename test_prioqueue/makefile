CC = g++
CFLAGS = -Wall -Werror -O3 -std=c++11

SRC = prioqueue_test.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = prioqueue_test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) ../*.o *.o $(TARGET)

test: $(TARGET)
	./$(TARGET)
