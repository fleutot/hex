/*------------------------------------------------------------------------------
Implementation of a hex board
------------------------------------------------------------------------------*/
#ifndef HEXBOARD_HPP_INCLUDED
#define HEXBOARD_HPP_INCLUDED

#include <vector>
#include "graph.hpp"
#include "player.hpp"

using namespace std;

extern const int nb_players;

class HexBoard {
public:
    HexBoard(unsigned size);
    HexBoard(HexBoard& origin);
    ~HexBoard();

    // Play a move, returns false if the move was unauthorized.
    bool play(const unsigned col, const unsigned row, const Player player);
    bool play(const pair<unsigned, unsigned> coord, const Player player) {
        return play(coord.first, coord.second, player);
    }

    // Return a list of all unoccupied slots.
    const vector< pair<unsigned, unsigned> >& unoccupied_list_get() const {
        return unoccupied_list;
    }

    unsigned size_get() const {
        return size;
    }

    unsigned refs_to_board_increment() {
        return ++(*nb_refs_to_board);
    }

    unsigned refs_to_board_decrement() {
        if (*nb_refs_to_board > 0) {
            --(*nb_refs_to_board);
        }
        return *nb_refs_to_board;
    }

    bool occupied_check(const unsigned col, const unsigned row) const {
        // map is a vector of rows.
        return occupied_map[row][col].is_player();
    }

    friend ostream& operator<< (ostream& os, const HexBoard& h);
    // Print the intermediate row that shows links between the positions of
    // different rows. This is meant to be used by operator<< only.
    void slanted_links_row_print(ostream& os, const unsigned slot_width,
                                 const unsigned row_index) const;

    // Implemented for automated test purposes (see unit tests).
    bool sanity_check();
    int nb_trees_get(const Player player) {
        player_select(player);
        return trees->size();
    }

protected:
    unsigned size;

    // Dynamically allocated, to make the default copy constructor do a shallow
    // copy. Copies of HexBoard do not modify the board graph itself, there is
    // therefore no reason to make a deep copy of it, all copies of HexBoard can
    // use the same graph as the original.
    Graph *board;
    // Keep track of how many HexBoard instances use the graph above. Only
    // delete the graph if the last HexBoard instance using it is destroyed.
    unsigned *nb_refs_to_board;

    // Virtual nodes to represent the sides of the board. These are vertices in
    // the graph, and connected to every position in the side. A winning
    // condition is if west is connected to east, or north to south.
    int west, east, north, south;

    // Map of who occupies which slot.
    vector< vector<Player> > occupied_map;
    // List of slots that are not occupied by anyone. This is redundant
    // information, but maintaining it here in this form removes the need to
    // compute it many times when running an AI on the board.
    vector< pair<unsigned, unsigned> > unoccupied_list;

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
    unsigned coord2lin(unsigned col, unsigned row) const {
        return row * size + col;
    }

    // Convert a linear index to a column and a row coordinates.
    void lin2coord(const unsigned lin, unsigned& col, unsigned& row) const {
        row = lin / size;
        col = lin % size;
    }

    // Update the player's forest of trees with the newly played position col,
    // row. Return the index of the newly updated tree.
    unsigned update_trees(const unsigned col, const unsigned row);

    // If there is one, find the index of the tree in the given forest, which
    // contains the vertex passed as first parameter.
    // Return false if no tree was found, true otherwise.
    bool containing_tree_get(const int vertex_name, unsigned& found_tree_index);

    // Merge two trees into one, if not the same. One of the trees is destroyed
    // as a result of the merge.
    unsigned trees_merge(unsigned index_a, unsigned index_b);

    // Check if two nodes are connected in the tree passed as parameter.
    bool connected_in_tree_check(const int node_a, const int node_b,
                                 const unsigned tree_index);
};

ostream& operator<< (ostream& os, const HexBoard& h);
void move_print(const pair<unsigned, unsigned> move);

#endif // HEXBOARD_HPP_INCLUDED
