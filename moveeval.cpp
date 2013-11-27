/*------------------------------------------------------------------------------
Hex Board Evaluator
hexboardeval.cpp
------------------------------------------------------------------------------*/
#include <algorithm>
#include <vector>
#include <utility>

#include "hexboard.hpp"
#include "moveeval.hpp"

using namespace std;

static const unsigned max_simulations_per_test_move = 1000u;

pair<unsigned, unsigned> MoveEvaluator::best_move_calculate()
{
    const vector< pair<unsigned, unsigned> > free_slots
                  = real_board.unoccupied_list_get();

    unsigned nb_simulations_per_move = nb_total_simulations
            / free_slots.size();
    if (nb_simulations_per_move > max_simulations_per_test_move) {
        nb_simulations_per_move = max_simulations_per_test_move;
    }
    cout << " (" << nb_simulations_per_move << " simulations per tested move)"
         << flush;

    for (auto test_coord: free_slots) {
        // Run all the simulations with this test move first, keeping track of
        // the number of times it led to a win (score).
        int score = 0;
        for (unsigned mc_run = 0; mc_run < nb_simulations_per_move; ++mc_run) {
            HexBoard test_board(real_board);    // Fresh copy of the board.
            // Play the test move first.
            bool win = test_board.play(test_coord, tested_player);
            if (win) {
                // No need to run a simulation, this is the best move, winning.
                return test_coord;
            }
            // Now play all other positions randomly until the board is full.
            // The first random move after the test position is done by the
            // other player.
            test_board.fill_up(tested_player.other());
            if (test_board.win_check(tested_player)) {
                ++score;
            }
        }
        if (score > best_score) {
            best_score = score;
            best_coord = test_coord;
        }
    }
    return best_coord;
}
