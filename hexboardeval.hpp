/*------------------------------------------------------------------------------
Hex Board Evaluator
hexboardeval.hpp
------------------------------------------------------------------------------*/
#ifndef HEXBOARDEVAL_H_INCLUDED
#define HEXBOARDEVAL_H_INCLUDED

#include <climits>

#include "hexboard.hpp"

// The HexBoardEvaluator class is a type of HexBoard on which calculations are
// run. Random moves are added and evaluated with brute force.
// The instances of this class are meant to be constructed with an actual
// HexBoard object, on which the best next move is to be calculated.
class HexBoardEvaluator: public HexBoard{
public:
    // parameters:
    // game_base: the base situation, HexGame with the moves already played.
    // nb_simulations: number of simulations to run per test move.
    // max_test_moves: max number of test moves to evaluate. If larger than the
    // number of free positions, evaluate all free positions.
    HexBoardEvaluator(const HexBoard& base_board,
                      const unsigned nb_simulations = 1000,
                      const unsigned max_test_moves = UINT_MAX):
        HexBoard(base_board),   // Base class default copy constructor.
        nb_test_moves(max_test_moves),
        nb_simulations_per_move(nb_simulations) {}

    // Generate a list of random moves, evaluate them, and return the best.
    // return value: the graph vertex index of the best move foundx.
    unsigned best_move_calculate();

protected:
    // The number of next moves to evaluate. If larger than the number of free
    // positions, test all free positions.
    unsigned nb_test_moves;
    // For each test move, run this number of monte carlo simulations to
    // evaluate the quality of the move.
    unsigned nb_simulations_per_move;
    // Store the quality (double) of each test move (unsigned, the node number
    // of the position under test).
    //unordered_map<unsigned, double> quality_map;
};

#endif // HEXBOARDEVAL_H_INCLUDED
