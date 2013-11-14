/*------------------------------------------------------------------------------
Implementation of a hex board
------------------------------------------------------------------------------*/
#include <cctype>
#include <iostream>
#include <iomanip>

#include "hexboard.hpp"
#include "graph.hpp"

using namespace std;

const int nb_players = 2;

static const char player2char[] = {
    [Player::NONE]  = '.',
    [Player::O]     = 'O',
    [Player::X]     = 'X'
};

HexBoard::HexBoard(unsigned size): size(size), board(size * size)
{
    occupied_map.resize(size);
    for (auto it = occupied_map.begin(); it != occupied_map.end(); ++it) {
        it->resize(size, Player::NONE);
    }

    // Create the edges of the graph. Besides special cases around the sides of
    // the board, create three edges for each vertix: 3 o'clock, 5 o'clock, 7
    // o'clock. Special cases are the west, east, and south sides of the board.

    // West and east sides (not the south corners, which are done with south side):
    for (unsigned row = 0; row < size - 1; ++row) {
        // West edge
        unsigned vertix = coord2lin(0, row);
        unsigned vertix_3oclock = coord2lin(1, row);
        unsigned vertix_5oclock = coord2lin(0, row + 1);
        board.edge_add(vertix, vertix_3oclock);
        board.edge_add(vertix, vertix_5oclock);
        // no 7 o'clock for this side of the board.

        // East edge
        vertix = coord2lin(size - 1, row);
        vertix_5oclock = coord2lin(size - 1, row + 1);
        unsigned vertix_7oclock = coord2lin(size - 2, row + 1);
        board.edge_add(vertix, vertix_5oclock);
        board.edge_add(vertix, vertix_7oclock);
        // no 3 o'clock for this side of the board.
    }
    // South edge of the board.
    for (unsigned col = 0; col < size - 1; ++col) {
        // 3 o'clock edge.
        board.edge_add(coord2lin(col, size - 1), coord2lin(col + 1, size - 1));
    }
    // All other vertices.
    for (unsigned col = 1; col < size - 1; ++col) {
        for (unsigned row = 0; row < size - 1; ++row) {
            unsigned vertix = coord2lin(col, row);
            unsigned vertix_3oclock = coord2lin(col + 1, row);
            unsigned vertix_5oclock = coord2lin(col, row + 1);
            unsigned vertix_7oclock = coord2lin(col - 1, row + 1);
            board.edge_add(vertix, vertix_3oclock);
            board.edge_add(vertix, vertix_5oclock);
            board.edge_add(vertix, vertix_7oclock);
        }
    }
}
    

HexBoard::~HexBoard()
{
    // do nothing. The build process wanted a definition here, rather than an
    // empty one in the class implementation. It would be interesting to know
    // why, but there is no time now.
}

bool HexBoard::sanity_check()
{
    return (board.nb_vertices_get() == (size * size)
            && board.nb_edges_get() == (3 * size * (size - 1) - (size - 1))
            && occupied_map.size() == size
            && occupied_map[0].size() == size
            && occupied_map[size - 1].size() == size
        );
}

bool HexBoard::play(unsigned col, unsigned row, Player player)
{
    return false;
}

ostream& operator<< (ostream& os, HexBoard& board)
{
    const int slot_width = 4;

    // column labels
    os << setw(slot_width) << " ";
    for (unsigned i = 0; i < board.size; ++i) {
        os << setw(slot_width) << static_cast<char>(i + 'A');
    }
    os << endl;

    unsigned row_index = 0;
    for (auto row: board.occupied_map) {
        // offset to make the board slanted.
        for (unsigned i = 0; i < row_index; ++i) {
            os << setw(slot_width / 2) << " ";
        }
        os << setw(slot_width) << ++row_index; // row label
        for (auto slot_value: row) {
            os << setw(slot_width) << player2char[static_cast<short>(slot_value)];
        }
        os << setw(slot_width) << row_index << endl; // repeat the row label
    }

    // repeat column labels. One extra half slot to align with the previous row.
    for (unsigned i = 0; i < row_index + 1; ++i) {
        os << setw(slot_width / 2) << " ";
    }
    for (unsigned i = 0; i < board.size; ++i) {
        os << setw(slot_width) << static_cast<char>(i + 'A');
    }

    return os;
}
