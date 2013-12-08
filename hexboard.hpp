/*------------------------------------------------------------------------------
Implementation of a hex board
------------------------------------------------------------------------------*/
#ifndef HEXBOARD_HPP_INCLUDED
#define HEXBOARD_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <random>
#include <vector>
#include "graph.hpp"
#include "player.hpp"

using std::vector;

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

    // Remove a stone at the given position. Requires that a stone actually is
    // at the given position.
    void unplace(const unsigned col, const unsigned row);
    void unplace(const pair<unsigned, unsigned> coord) {
        unplace(coord.first, coord.second);
    }

    // Destructive. Once this function is called, the content of the member
    // variable occupied_X or _O is not valid. Note the the destroyed map is
    // that of thet *other* player than the one passed as parameter.
    // Use the function occupied_save and occupied_restore to revert to a known
    // state.
    // The unoccupied member has been shuffled, but its content has not been
    // otherwise changed.
    bool fill_up_half_and_win_check(Player player);
    void fill_up(const Player first_player);

    bool win_check(const Player player);

    vector<uint16_t>& occupied_save(const Player player) {
        return (player.get() == player_e::X)
            ? occupied_X
            : occupied_O;
    }

    void occupied_restore(const Player player, vector<uint16_t>& src) {
        if (player.get() == player_e::X) {
            occupied_X = src;
        } else {
            occupied_O = src;
        }
    }

    // Return a list of all unoccupied slots. The reference may modify the
    // content of the class member! This is a bit dangerous, but gives a
    // significant optimization opportunity.
    vector< pair<unsigned, unsigned> >& unoccupied_list_get() {
        return unoccupied_list;
    }

    unsigned size_get() const {
        return size;
    }

    bool occupied_check(const unsigned col, const unsigned row) const {
        // map is a vector of rows.
        return occupied_map[row][col].is_player();
    }

    void occupied_list_get(const Player player, vector<int>& list);

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

    // These are used for an optimized monte-carlo simulation. A bit to 1 means
    // this player has a stone at that position. For occupied_X, the first
    // uint16 is the highest row of the board. For occupied_O, it is the
    // left-most column.
    // bit 0 of the the uint16 is column 0 for occupied_X, row 0 for
    // occupied_O. Note that if you think of bit 0 as the right-most bit, this
    // means these uint16s are left-right mirrored compared to the actual board.
    // This helps optimization, and gives the same number for bit and column/row.
    vector<uint16_t> occupied_X;
    vector<uint16_t> occupied_O;

    // Indeces to the winning board sides (virtual nodes) of the current player.
    int side_a, side_b;

    mt19937 random_engine;

    // Update the variable trees to point to the correct trees_O or trees_X
    // depending on the current player.
    inline void player_select(const Player player);

    void occupied_set(unsigned col, unsigned row, Player player, int value = 1);
    void occupied_reset(unsigned col, unsigned row, Player player) {
        occupied_set(col, row, player, 0);
    }

    void occupied_X_set(unsigned col, unsigned row);
    void occupied_O_set(unsigned col, unsigned row);
    void (HexBoard::*occupied_player_set)(unsigned, unsigned);

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
};

ostream& operator<< (ostream& os, const HexBoard& h);
void move_print(const pair<unsigned, unsigned> move);

#endif // HEXBOARD_HPP_INCLUDED
