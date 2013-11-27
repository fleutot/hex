/*------------------------------------------------------------------------------
Implementation of a hex board
------------------------------------------------------------------------------*/
#ifndef HEXBOARD_HPP_INCLUDED
#define HEXBOARD_HPP_INCLUDED

#include <memory>
#include <vector>
#include "graph.hpp"
#include "player.hpp"

using namespace std;

extern const int nb_players;

class HexBoard {
public:
    HexBoard(unsigned size);

    // Play a move, returns true if the game was won.
    bool play(const unsigned col, const unsigned row, const Player player);
    bool play(const pair<unsigned, unsigned> coord, const Player player) {
        return play(coord.first, coord.second, player);
    }

    // Place a move but do not check for win.
    void place(const unsigned col, const unsigned row, const Player player);
    void place(const pair<unsigned, unsigned> coord, const Player player) {
        return place(coord.first, coord.second, player);
    }

    bool win_check(const Player player);

    // Return a list of all unoccupied slots.
    const vector< pair<unsigned, unsigned> >& unoccupied_list_get() const {
        return unoccupied_list;
    }

    unsigned size_get() const {
        return size;
    }

    bool occupied_check(const unsigned col, const unsigned row) const {
        // map is a vector of rows.
        return occupied_map[row][col].is_player();
    }

    vector<int> occupied_list_get(const Player player);

    friend ostream& operator<< (ostream& os, const HexBoard& h);
    // Print the intermediate row that shows links between the positions of
    // different rows. This is meant to be used by operator<< only.
    void slanted_links_row_print(ostream& os, const unsigned slot_width,
                                 const unsigned row_index) const;

    // Implemented for automated test purposes (see unit tests).
    bool sanity_check();

protected:
    unsigned size;

    // Dynamically allocated, to make the default copy constructor do a shallow
    // copy. Copies of HexBoard do not modify the board graph itself, there is
    // therefore no reason to make a deep copy of it, all copies of HexBoard can
    // use the same graph as the original.
    shared_ptr<Graph> board;

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

    // Recursive path search used by win_check().
    bool win_search_recursive(const int node, vector<int>& unvisited);
};

ostream& operator<< (ostream& os, const HexBoard& h);
void move_print(const pair<unsigned, unsigned> move);

#endif // HEXBOARD_HPP_INCLUDED
