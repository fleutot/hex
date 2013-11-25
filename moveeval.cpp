/*------------------------------------------------------------------------------
Hex Board Evaluator
hexboardeval.cpp
------------------------------------------------------------------------------*/
#include <algorithm>
#include <utility>

#include "hexboard.hpp"
#include "moveeval.hpp"

using namespace std;

pair<unsigned, unsigned> MoveEvaluator::best_move_calculate()
{
    // So far, testing all unoccupied positions.
    for (auto test_coord: real_board.unoccupied_list_get()) {
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
            // Now play all other positions randomly until there is a win.
            // The first random move after the test position is done by the
            // other player.
            win = random_play_until_win(test_board, tested_player);
            if (win) {
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

// Parameter "player" must be the last player to have played, since it will be
// swapped to the other player before making the next move.
bool MoveEvaluator::random_play_until_win(HexBoard& test_board, Player player)
{
    vector< pair<unsigned, unsigned> > free_pos
        = test_board.unoccupied_list_get();

    // Order the future moves randomly.
    random_shuffle(free_pos.begin(), free_pos.end());

    bool win;
    do {
        pair<unsigned, unsigned> next_move = free_pos.back();
        free_pos.pop_back();
        player.swap();
        win = test_board.play(next_move, player);
    } while (!win);

    // Return true if the winner of the random moves was the player we were
    // testing for.
    return player == tested_player;
}
