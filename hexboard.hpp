/*------------------------------------------------------------------------------
Implementation of a hex board
------------------------------------------------------------------------------*/
#ifndef HEXBOARD_HPP_INCLUDED
#define HEXBOARD_HPP_INCLUDED

#include <vector>
#include "graph.hpp"

using namespace std;

extern const int nb_players;
extern const char player2char[];

enum class Player: int {
    NONE,
    O,  // This is capital letter o.
    X
};

class HexBoard {
public:
    HexBoard(unsigned size);
    ~HexBoard();

    // Play a move, returns false if the move was unauthorized.
    bool play(const unsigned col, const unsigned row, const Player player);

    // Implemented for automated test purposes (see unit tests).
    bool sanity_check();
    int nb_trees_get(const Player player) {
        player_select(player);
        return trees->size();
    }

    unsigned size_get() const {
        return size;
    }

    bool occupied_check(const unsigned col, const unsigned row) const {
        // map is a vector of rows.
        return occupied_map[row][col] != Player::NONE;
    }

    friend ostream& operator<< (ostream& os, const HexBoard& h);
    // Print the intermediate row that shows links between the positions of
    // different rows. This is meant to be used by operator<< only.
    void slanted_links_row_print(ostream& os, const unsigned slot_width,
                                 const unsigned row_index) const;

private:
    unsigned size;
    Graph board;

    // Virtual nodes to represent the sides of the board. These are vertices in
    // the graph, and connected to every position in the side. A winning
    // condition is if west is connected to east, or north to south.
    int west, east, north, south;

    vector< vector<Player> > occupied_map;

    // Collection of trees formed by each players' play.
    vector< vector<int> > trees_O;
    vector< vector<int> > trees_X;

    // Used to point to trees_O or trees_X, depending on the current player.
    vector< vector<int> > *trees = &trees_O;

    // Indeces to the winning board sides (virtual nodes) of the current player.
    int side_a, side_b;

    // Update the variable trees to point to the correct trees_O or trees_X
    // depending on the current player.
    void player_select(const Player player);

    // Convert a column and row pair to a linear index, which is used as a node
    // name in the graph.
    unsigned coord2lin(unsigned col, unsigned row) {
        return row * size + col;
    }

    // Update the player's forest of trees with the newly played position col,
    // row. Return the index of the newly updated tree.
    unsigned update_trees(const unsigned col, const unsigned row,
                          const Player player);

    // If there is one, find the index of the tree in the given forest, which
    // contains the vertex passed as first parameter.
    // Return false if no tree was found, true otherwise.
    bool containing_tree_get(const int vertex_name, unsigned& found_tree_index);

    // Merge two trees into one, if not the same. Three B is destroyed after
    // having been merged into A.
    void trees_merge(unsigned& index_a, unsigned index_b);

    // Check if two nodes are connected in the tree passed as parameter.
    bool connected_in_tree_check(const int node_a, const int node_b,
                                 const unsigned tree_index);
};

ostream& operator<< (ostream& os, const HexBoard& h);

#endif // HEXBOARD_HPP_INCLUDED
