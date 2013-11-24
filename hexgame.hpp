/*------------------------------------------------------------------------------
Hex game class implementation
------------------------------------------------------------------------------*/
#ifndef HEXGAME_H_INCLUDED
#define HEXGAME_H_INCLUDED

#include <climits>
#include <iostream>

#include "hexboard.hpp"

class HexGame {
public:
    HexGame(unsigned size, Player start_player = Player::X):
        board(size), current_player(start_player), winner(Player::NONE) {}

    ~HexGame() {}

    // Show the game introducttion header.
    void start_prompt();

    // Let the user decide which players are AI or human.
    void player_setup_prompt();

    bool next_prompt_and_play();   // returns true if the game is finished.

    void winner_print();

protected:
    HexBoard board;
    enum class PlayerType { NONE, AI, HUMAN};
    PlayerType player_X_type, player_O_type;
    Player current_player;
    Player winner;

    void human_prompt_play();

    bool input_get(pair<unsigned, unsigned>& move);
    void player_switch();
};

#endif // HEXGAME_H_INCLUDED
