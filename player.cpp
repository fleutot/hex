/*------------------------------------------------------------------------------
Player class implementation
player.hpp
------------------------------------------------------------------------------*/
#include <iostream>

using namespace std;

#include "player.hpp"

ostream& operator<<(ostream& os, const Player& player)
{
    // This commented implementation is not supported by g++, who says:
    // "sorry, unimplemented: non-trivial designated initializers not supported"
    // static const char player2char[] = {
    //     [player_e::NONE]    = '.',
    //     [player_e::O]       = 'O',
    //     [player_e::X]       = 'X'
    // };
    // os << player2char[static_cast<int>(player.get())];

    if (player.get() == player_e::NONE) {
        os << ".";
    } else if (player.get() == player_e::X) {
        os << "X";
    } else if (player.get() == player_e::O) {
        os << "O";
    } else {
        cerr << __func__ << ": error in player input variable." << endl;
    }
    return os;
}
