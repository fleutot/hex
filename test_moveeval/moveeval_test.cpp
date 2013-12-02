/*----------------------------------------------------------------------------
Unit test for the class HexGame and HexGameEvaluator
----------------------------------------------------------------------------*/

// Module under test
#include "../moveeval.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "../hexboard.hpp"

using namespace std;

//******************************************************************************
// Module variables
//******************************************************************************
static const Player player_O(player_e::O);
static const Player player_X(player_e::X);

//******************************************************************************
// Function prototypes
//******************************************************************************
void test_moveeval_constructor();
void test_moveeval_best_move();

int main(void)
{
    test_moveeval_constructor();
    test_moveeval_best_move();
    cout << endl << "All tested passed." << endl;
    return 0;
}

void test_moveeval_constructor()
{
    cout << __func__ << endl;
    HexBoard board(3);

    board.play(0, 1, player_O);
    board.play(1, 1, player_O);

    MoveEvaluator evaluator(board, player_O);
}

void test_moveeval_best_move()
{
    cout << __func__ << endl;
    HexBoard board(3);
    board.play(0, 0, player_O);
    board.play(1, 0, player_O);

    MoveEvaluator evaluator(board, player_O);
    pair<unsigned, unsigned> best_move = evaluator.best_move_calculate();

    assert((best_move.first == 2) && (best_move.second == 0));
}
