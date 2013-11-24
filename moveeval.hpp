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
    // base_board: the board with moves that are actually played (as opposed to
    // only under test).
    // nb_simulations: number of simulations to run per test move.
    // max_test_moves: max number of test moves to evaluate. If larger than the
    // number of free positions, evaluate all free positions.
    MoveEvaluator(const HexBoard& base_board,
                  const Player current_player,
                  const unsigned nb_simulations = 1000,
                  const unsigned max_test_moves = UINT_MAX):
        real_board(base_board),
        tested_player(current_player),
        nb_test_moves(max_test_moves),
        nb_simulations_per_move(nb_simulations),
        best_score(-1)
    {
        // Init to nonsense value.
        best_coord = make_pair(base_board.size_get(), base_board.size_get());
    }

    // Play a number of possible moves, evaluate them, and return the best.
    // return value: the coordinate of the best move found.
    pair<unsigned, unsigned> best_move_calculate();

protected:
    bool random_play_until_win(HexBoard& test_board, Player player);

    const HexBoard& real_board;

    const Player tested_player;
    // The number of next moves to evaluate. If larger than the number of free
    // positions, test all free positions.
    unsigned nb_test_moves;
    // For each test move, run this number of monte carlo simulations to
    // evaluate the quality of the move.
    unsigned nb_simulations_per_move;

    int best_score;
    pair<unsigned, unsigned> best_coord;
    // Store the quality (double) of each test move (unsigned, the node number
    // of the position under test).
    //unordered_map<unsigned, double> quality_map;
};

#endif // MOVEEVAL_H_INCLUDED
