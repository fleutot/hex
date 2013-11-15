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
void test_hexboard_win_1(void);
void test_hexboard_win_2(void);
void test_hexboard_win_3(void);

int main(void)
{
    test_hexboard_constructor();
    test_hexboard_display();
    test_hexboard_play();
    test_hexboard_win_1();
    test_hexboard_win_2();
    test_hexboard_win_3();
    cout << "All tested passed (but user check needed!)." << endl;
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
    board.play(1, 1, Player::O);
    assert(board.nb_trees_get(Player::O) == 1 + 2); // includes virtual nodes.
    cout << board << endl << endl;

    cout << "One unauthorized move: " << endl;
    assert(!board.play(1, 1, Player::X));

    cout << "Compare this board with the one above, they must be identical:" << endl;
    cout << board << endl;

    cout << "Two unauthorized moves: " << endl;
    assert(!board.play(1, 3, Player::O));
    assert(!board.play(3, 1, Player::O));

    // Check if trees seem correct so far, after erroneous play.
    assert(board.nb_trees_get(Player::O) == 1 + 2); // with virtual nodes.
    assert(board.nb_trees_get(Player::X) == 0 + 2); // with virtual nodes.

    // Test merging two separate trees into one.
    board.play(0, 1, Player::X);
    assert(board.nb_trees_get(Player::X) == 1 + 2); // with virtual nodes.
    board.play(0, 2, Player::X);
    assert(board.nb_trees_get(Player::X) == 2); // one virtual now connected

    board.play(0, 0, Player::X);
    assert(board.nb_trees_get(Player::X) == 1);
}

void test_hexboard_win_1(void)
{
    cout << __func__ << endl;

    bool win = false;
    HexBoard board(3);

    win = board.play(1, 1, Player::X);
    assert(!win);
    win = board.play(0, 0, Player::O);
    assert(!win);
    win = board.play(1, 2, Player::X);
    assert(!win);
    win = board.play(2, 0, Player::O);
    assert(!win);
    win = board.play(2, 2, Player::X);
    assert(!win);
    win = board.play(1, 0, Player::O);
    assert(win);
    cout << board << endl;
}

void test_hexboard_win_2(void)
{
    cout << __func__ << endl;

    bool win = false;
    HexBoard board(3);

    win = board.play(1, 1, Player::X);
    assert(!win);
    win = board.play(0, 0, Player::O);
    assert(!win);
    win = board.play(1, 2, Player::X);
    assert(!win);
    win = board.play(0, 1, Player::O);
    assert(!win);
    win = board.play(1, 0, Player::X);
    assert(win);
    win = board.play(0, 2, Player::O);
    assert(!win);
}

void test_hexboard_win_3(void)
{
    cout << __func__ << endl;

    bool win = false;
    HexBoard board(2);

    win = board.play(0, 0, Player::O);
    assert(!win);
    win = board.play(1, 0, Player::X);
    assert(!win);
    win = board.play(0, 1, Player::O);
    assert(!win);
    win = board.play(1, 1, Player::X);
    assert(win);
}
