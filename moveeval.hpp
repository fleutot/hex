/*------------------------------------------------------------------------------
Move Evaluator for Hex
moveeval.hpp
------------------------------------------------------------------------------*/
#ifndef MOVEEVAL_H_INCLUDED
#define MOVEEVAL_H_INCLUDED

#include <climits>

#include "hexboard.hpp"

class MoveEvaluator {
public:
    // parameters:
    // base_board: the board with moves that are actually played.
    // current_player: next player to place a stone.
    // max_nb_simulations: max number of simulations to run (~ nb test moves * nb
    // simulations per move).
    // max_nb_simulations_per_test: max number of Monte-Carlo to run per test
    // move.
    MoveEvaluator(HexBoard& base_board,
                  const Player current_player,
                  const unsigned max_nb_simulations = 100000u,
                  const unsigned max_nb_simulations_per_test = 2000u):
        board(base_board),
        tested_player(current_player),
        max_nb_total_simulations(max_nb_simulations),
        max_nb_simulations_per_test(max_nb_simulations_per_test),
        best_score(-1)
    {
        // Init to nonsense value.
        best_coord = make_pair(base_board.size_get(), base_board.size_get());
    }

    // Play a number of possible moves, evaluate them, and return the best.
    // return value: the coordinate of the best move found.
    pair<unsigned, unsigned> best_move_calculate();

protected:
    HexBoard& board;

    const Player tested_player;

    // Max total number of simualtions that can be run, all test moves times the
    // number of simulations per test move. This is done to limit the time spent
    // evaluating the best move.
    unsigned max_nb_total_simulations;

    unsigned max_nb_simulations_per_test;

    int best_score;
    pair<unsigned, unsigned> best_coord;
    // Store the quality (double) of each test move (unsigned, the node number
    // of the position under test).
    //unordered_map<unsigned, double> quality_map;
};

#endif // MOVEEVAL_H_INCLUDED
