/*----------------------------------------------------------------------------
Unit test for the class HexGame and HexGameEvaluator
----------------------------------------------------------------------------*/

// Module under test
#include "../hexboardeval.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "../hexboard.hpp"

using namespace std;

//******************************************************************************
// Function prototypes
//******************************************************************************
void test_hexboardeval_constructor();

int main(void)
{
    test_hexboardeval_constructor();
    cout << "All tested passed (but user check needed!)." << endl;
    return 0;
}

void test_hexboardeval_constructor()
{
    HexBoard board(3);

    board.play(0, 1, Player::O);
    board.play(1, 1, Player::O);

    HexBoardEvaluator eval_board(board, 1, 1);

    bool win = eval_board.play(2, 1, Player::O);
    assert(win);

}

