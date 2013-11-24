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
// Function prototypes
//******************************************************************************
void test_moveeval_constructor();
void test_moveeval_best_move();

int main(void)
{
    test_moveeval_constructor();
    test_moveeval_best_move();
    cout << "All tested passed." << endl;
    return 0;
}

void test_moveeval_constructor()
{
    HexBoard board(3);

    board.play(0, 1, Player::O);
    board.play(1, 1, Player::O);

    MoveEvaluator evaluator(board, 1);
}

void test_moveeval_best_move()
{
    HexBoard board(3);
    board.play(0, 0, Player::O);
    board.play(1, 0, Player::O);

    MoveEvaluator evaluator(board);
    unsigned best_node = evaluator.best_move_calculate();

    assert(best_node == 2);
}
