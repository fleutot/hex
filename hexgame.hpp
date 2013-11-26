/*------------------------------------------------------------------------------
Hex game class implementation
------------------------------------------------------------------------------*/
#ifndef HEXGAME_H_INCLUDED
#define HEXGAME_H_INCLUDED

#include <climits>
#include <iostream>

#include "hexboard.hpp"

enum class PlayerType { NONE, AI, HUMAN};

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
    PlayerType player_X_type, player_O_type;

    Player current_player;
    Player winner;

    PlayerType current_player_type_get();

    void human_prompt_play();

    bool human_input_get(pair<unsigned, unsigned>& move);
    void ai_input_get(pair<unsigned, unsigned>& move);

    bool char_to_player_type_set(const char c, PlayerType& type);
};

#endif // HEXGAME_H_INCLUDED
