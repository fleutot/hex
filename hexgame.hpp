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
    HexGame(unsigned size, player_e start_player = player_e::X):
        board(size)
    {
        current_player.set(start_player);
        winner.set(player_e::NONE);
    }

    ~HexGame() {}

    // Show the game introduction header.
    void start_prompt();

    // Let the user decide which players are AI or human.
    void player_setup_prompt_and_set();

    bool next_prompt_and_play();   // returns true if the game is finished.

    void winner_print();

protected:
    HexBoard board;
    enum class PlayerType { NONE, AI, HUMAN};
    PlayerType player_X_type, player_O_type;
    PlayerType current_player_type;
    Player current_player;
    Player winner;

    void human_prompt_play();

    bool input_get(pair<unsigned, unsigned>& move);

    bool char_to_player_type_set(const char c, PlayerType& type);
};

#endif // HEXGAME_H_INCLUDED
