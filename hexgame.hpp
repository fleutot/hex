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

//class HexGameEvaluator: public HexGame {
//public:
//    // parameters:
//    // nb_simulations: number of simulations to run per test move.
//    // max_test_moves: max number of test moves to evaluate. If larger than the
//    // number of free positions, evaluate all free positions.
//    HexGameEvaluator(const unsigned nb_simulations,
//                     const unsigned max_test_moves = UINT_MAX):
//        nb_test_moves(max_test_moves),
//        nb_simulations_per_move(nb_simulations) {}
//
//    // Generate a list of random moves, evaluate them, and return the best.
//    // return value: the graph vertex index of the best move foundx.
//    unsigned best_move_calculate();
//
//protected:
//    // The number of next moves to evaluate. If larger than the number of free
//    // positions, test all free positions.
//    unsigned nb_test_moves;
//    // For each test move, run this number of monte carlo simulations to
//    // evaluate the quality of the move.
//    unsigned nb_simulations_per_move;
//    // Store the quality (double) of each test move (unsigned, the node number
//    // of the position under test).
//    unordered_map<unsigned, double> quality_map;
//
//};

#endif // HEXGAME_H_INCLUDED
