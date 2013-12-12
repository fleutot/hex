/*------------------------------------------------------------------------------
Hex game class implementation
------------------------------------------------------------------------------*/
#ifndef HEXGAME_H_INCLUDED
#define HEXGAME_H_INCLUDED

#include <climits>
#include <iostream>

#include "hexboard.hpp"

enum class PlayerType { NONE, AI, HUMAN, OPPONENT_AI};

class HexGame {
public:
    HexGame(unsigned size, unsigned simulations_per_test_move = 1000,
            unsigned max_simulations = UINT_MAX,
            player_e start_player = player_e::X):
        board(size), max_simulations(max_simulations),
        simulations_per_test_move(simulations_per_test_move)
    {
        current_player.set(start_player);
        winner.set(player_e::NONE);
    }
    ~HexGame() {}

    // ----- Interactive game section -----
    // Show the game introduction header.
    void start_prompt();
    // Let the user decide which players are AI or human.
    void player_setup_prompt_and_set();
    // returns true if the game is finished.
    bool next_prompt_and_play();
    void winner_print();

    // ----- Autoplay section -----
    int autoplay_handshake(const char color);
    // Set players for autoplay, this AI plays 'color'.
    void player_autoplay_setup(const char color);
    bool autoplay_next_move_play();


protected:
    HexBoard board;
    PlayerType player_X_type, player_O_type;

    Player current_player;
    Player winner;

    unsigned max_simulations;
    unsigned simulations_per_test_move;

    PlayerType current_player_type_get();

    void human_prompt_play();

    bool human_input_get(pair<unsigned, unsigned>& move);
    bool ai_input_get(pair<unsigned, unsigned>& move);
    int autoplay_input_move_make(pair<unsigned, unsigned>& move,
                                 double& milliseconds);
    void autoplay_move_print(pair<unsigned, unsigned> const& move,
                             const bool win, const double elapsed_milli);
    int autoplay_opponent_move_read(pair<unsigned, unsigned>& move,
                                    bool& give_up);
    void autoplay_capitulate_print();

    bool char_to_player_type_set(const char c, PlayerType& type);
};

#endif // HEXGAME_H_INCLUDED
