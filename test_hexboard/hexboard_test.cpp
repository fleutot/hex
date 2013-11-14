/*----------------------------------------------------------------------------
Unit test for the class HexBoard
----------------------------------------------------------------------------*/

// Module under test
#include "../hexboard.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

//******************************************************************************
// Function prototypes
//******************************************************************************
void test_hexboard_constructor(void);
void test_hexboard_display(void);
void test_hexboard_play(void);

int main(void)
{
    test_hexboard_constructor();
    test_hexboard_display();
    test_hexboard_play();
    return 0;
}

void test_hexboard_constructor(void)
{
    cout << __func__ << endl;
    HexBoard board(10);
    assert(board.sanity_check());
}

void test_hexboard_display(void)
{
    cout << __func__ << endl;
    HexBoard board(5);
    cout << board << endl;
}

void test_hexboard_play(void)
{
    cout << __func__ << endl;
    HexBoard board(3);
    assert(board.play(1, 1, Player::O));
    cout << board << endl << endl;

    cout << "One unauthorized move: " << endl;
    assert(!board.play(1, 1, Player::X));
    cout << "Compare this board with the one above, they must be identical:" << endl;
    cout << board << endl;

    cout << "Two unauthorized moves: " << endl;
    assert(!board.play(1, 3, Player::O));
    assert(!board.play(3, 1, Player::O));
    assert(board.nb_trees_get(Player::O) == 1);
    assert(board.nb_trees_get(Player::X) == 0);

    // Test merging two separate trees into one.
    board.play(0, 0, Player::X);
    board.play(2, 0, Player::X);
    assert(board.nb_trees_get(Player::X) == 2);
    board.play(1, 0, Player::X);
    assert(board.nb_trees_get(Player::X) == 1);
}
