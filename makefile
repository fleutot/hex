CC = g++
CFLAGS = -Wall -Werror -O3 -std=c++11

SRC_ASP = graph.cpp shortestpathalgo.cpp average_shortest_path.cpp
OBJ_ASP = $(SRC_ASP:.cpp=.o)
TARGET_ASP = average_shortest_path

SRC_MST = graph.cpp mst_kruskal.cpp minimum_spanning_tree.cpp
OBJ_MST = $(SRC_MST:.cpp=.o)
TARGET_MST = minimum_spanning_tree

SRC_HEX = graph.cpp hexboard.cpp hexgame.cpp hex.cpp player.cpp moveeval.cpp
OBJ_HEX = $(SRC_HEX:.cpp=.o)
TARGET_HEX = hexjakt

all: $(TARGET_MST) $(TARGET_ASP) $(TARGET_HEX)

asp: $(TARGET_ASP)

mst: $(TARGET_MST)

$(TARGET_MST): $(OBJ_MST)
	$(CC) $(CFLAGS) $(OBJ_MST) -o $(TARGET_MST)

$(TARGET_ASP): $(OBJ_ASP)
	$(CC) $(CFLAGS) $(OBJ_ASP) -o $(TARGET_ASP)

$(TARGET_HEX): $(OBJ_HEX)
	$(CC) $(CFLAGS) $(OBJ_HEX) -o $(TARGET_HEX)

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o $(TARGET_MST) $(TARGET_ASP) $(TARGET_HEX)

