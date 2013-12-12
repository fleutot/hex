/*------------------------------------------------------------------------------
Hex Board Evaluator
hexboardeval.cpp
------------------------------------------------------------------------------*/
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <utility>

#include "hexboard.hpp"
#include "moveeval.hpp"

using namespace std;

pair<unsigned, unsigned> MoveEvaluator::best_move_calculate()
{
    const vector< pair<unsigned, unsigned> > free_slots
                  = board.unoccupied_list_get();

    // Limit the number of simulations in regard to the max total number of
    // simulations, and the max number of simulations per test move.
    unsigned nb_simulations_per_move = max_nb_total_simulations
            / free_slots.size();
    if (nb_simulations_per_move > max_nb_simulations_per_test) {
        nb_simulations_per_move = max_nb_simulations_per_test;
    }

    board.player_select(tested_player);

    for (auto test_coord: free_slots) {
        // Play the test move first.
        bool win = board.play(test_coord, tested_player);
        if (win) {
            // No need to run a simulation, this is a best move, winning.
            board.unplace(test_coord);
            return test_coord;
        }
        // Save the occupied map including test move, to restore between
        // simulations.
        vector<uint16_t> test_occupied = board.occupied_save();

        // Run all the simulations with this test move, keeping track of the
        // number of times it led to a win (score).
        int score = 0;
        for (unsigned mc_run = 0; mc_run < nb_simulations_per_move; ++mc_run) {
            // Play all positions randomly until the board is full.
            bool win = board.fill_up_half_and_win_check();
            if (win) {
                // A full board of hex has always exactly one winner.
                ++score;
            }
            board.occupied_restore(test_occupied);
        }

        if (score > best_score) {
            best_score = score;
            best_coord = test_coord;
        }
        board.unplace(test_coord);
    }

    return best_coord;
}
