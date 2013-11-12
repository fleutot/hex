/*------------------------------------------------------------------------------
Implementation of a hex board
------------------------------------------------------------------------------*/
#ifndef HEXBOARD_HPP_INCLUDED
#define HEXBOARD_HPP_INCLUDED

#include <vector>
#include "graph.hpp"

using namespace std;

extern const int nb_players;

enum class Player: int {
    NONE,
    O,  // This is capital letter o.
    X
};

class HexBoard {
public:
    HexBoard(int size);
    ~HexBoard();

    // Play a move, returns true if the move wins the game.
    bool play(unsigned col, unsigned row, Player player);

    // Implemented for automated test purposes.
    bool sanity_check();

    friend ostream& operator<< (ostream& os, HexBoard& h);

private:
    unsigned size;
    Graph board;
    vector< vector<Player> > occupied_map;

};

ostream& operator<< (ostream& os, HexBoard& h);

#endif // HEXBOARD_HPP_INCLUDED
