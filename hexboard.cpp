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

HexBoard::HexBoard(int size): size(size), board(size * size)
{
    occupied_map.resize(size);
    for (auto it = occupied_map.begin(); it != occupied_map.end(); ++it) {
        it->resize(size, Player::NONE);
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
