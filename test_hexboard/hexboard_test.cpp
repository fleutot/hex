/*----------------------------------------------------------------------------
Unit test for the class HexBoard
----------------------------------------------------------------------------*/

// Module under test
#include "../hexboard.hpp"
#include "../player.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

//******************************************************************************
// Module variables
//******************************************************************************
static const Player player_O(player_e::O);
static const Player player_X(player_e::X);

//******************************************************************************
// Function prototypes
//******************************************************************************
void test_hexboard_constructor(void);
void test_hexboard_copy_constructor(void);
void test_hexboard_display(void);
void test_hexboard_play(void);
void test_hexboard_unoccupied_list_get();
void test_hexboard_occupied_list_get();
void test_hexboard_fill_up();
void test_hexboard_win_1(void);
void test_hexboard_win_2(void);
void test_hexboard_win_3(void);
void test_hexboard_occupied_save_restore();

int main(void)
{
    srand(time(0));

    test_hexboard_constructor();
    test_hexboard_copy_constructor();
    test_hexboard_display();
    test_hexboard_play();
    test_hexboard_unoccupied_list_get();
    test_hexboard_occupied_list_get();
    test_hexboard_fill_up();
    test_hexboard_win_1();
    test_hexboard_win_2();
    test_hexboard_win_3();
    test_hexboard_occupied_save_restore();
    cout << "All tested passed (but user check needed!)." << endl;
    return 0;
}

void test_hexboard_constructor(void)
{
    cout << __func__ << endl;
    HexBoard board(10);
    assert(board.sanity_check());
}

void test_hexboard_copy_constructor(void)
{
    cout << __func__ << endl;
    HexBoard board(2);
    board.play(1, 1, player_O);
    HexBoard board_copy(board);
    bool win = board_copy.play(0, 1, player_O);
    assert(win);
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

    board.play(1, 1, player_O);
    cout << board << endl << endl;

    cout << "One unauthorized move: " << endl;
    assert(!board.play(1, 1, player_X));

    cout << "Compare this board with the one above, they must be identical:" << endl;
    cout << board << endl;

    cout << "Two unauthorized moves: " << endl;
    assert(!board.play(1, 3, player_O));
    assert(!board.play(3, 1, player_O));
}

void test_hexboard_unoccupied_list_get()
{
    cout << __func__ << endl;
    HexBoard board(2);
    board.play(0, 1, player_O);
    board.play(1, 0, player_O);

    vector< pair<unsigned, unsigned> > free_list = board.unoccupied_list_get();

    vector< pair<unsigned, unsigned> > expected_free;
    expected_free.push_back(make_pair(0, 0));
    expected_free.push_back(make_pair(1, 1));

    assert(free_list.size() == expected_free.size());

    for (unsigned i = 0; i < free_list.size(); ++i) {
        assert(free_list[i] == expected_free[i]);
    }
}

void test_hexboard_occupied_list_get()
{
    cout << __func__ << endl;
    HexBoard board(3);
    board.play(0, 1, player_O);
    board.play(1, 0, player_O);
    board.play(2, 0, player_O);
    board.play(2, 2, player_O);
    board.play(2, 1, player_X);

    vector<int> occupied_O = board.occupied_list_get(player_O);

    int expected[] = {1, 2, 3, 8};

    assert(occupied_O.size() == (sizeof(expected) / sizeof(expected[0])));

    for (unsigned i = 0; i < occupied_O.size(); ++i) {
        assert(occupied_O[i] == expected[i]);
    }

}

void test_hexboard_fill_up()
{
    cout << __func__ << endl;
    for (unsigned i = 0; i < 20; ++i) {
        HexBoard board(3);
        board.fill_up(player_O);

        vector<int> occupied_O = board.occupied_list_get(player_O);
        assert(occupied_O.size() == 5);

        vector<int> occupied_X = board.occupied_list_get(player_X);
        assert(occupied_X.size() == 4);
    }

    for (unsigned i = 0; i < 20; ++i) {
        HexBoard board(3);
        board.fill_up(player_X);

        vector<int> occupied_O = board.occupied_list_get(player_O);
        assert(occupied_O.size() == 4);

        vector<int> occupied_X = board.occupied_list_get(player_X);
        assert(occupied_X.size() == 5);
    }

    for (unsigned i = 0; i < 20; ++i) {
        HexBoard board(3);
        board.play(0, 1, player_O);

        board.fill_up(player_X);

        vector<int> occupied_O = board.occupied_list_get(player_O);
        assert(occupied_O.size() == 5);

        vector<int> occupied_X = board.occupied_list_get(player_X);
        assert(occupied_X.size() == 4);
    }
}

void test_hexboard_win_1(void)
{
    cout << __func__ << endl;

    bool win = false;
    HexBoard board(3);

    win = board.play(1, 1, player_X);
    assert(!win);
    assert(!board.win_check(player_X));

    win = board.play(0, 0, player_O);
    assert(!win);
    assert(!board.win_check(player_O));

    win = board.play(1, 2, player_X);
    assert(!win);
    assert(!board.win_check(player_X));

    win = board.play(2, 0, player_O);
    assert(!win);
    assert(!board.win_check(player_O));

    win = board.play(2, 2, player_X);
    assert(!win);
    assert(!board.win_check(player_X));

    win = board.play(1, 0, player_O);
    assert(win);
    assert(board.win_check(player_O));
}

void test_hexboard_win_2(void)
{
    cout << __func__ << endl;

    bool win = false;
    HexBoard board(3);

    win = board.play(1, 1, player_X);
    assert(!win);
    assert(!board.win_check(player_X));

    win = board.play(0, 0, player_O);
    assert(!win);
    assert(!board.win_check(player_O));

    win = board.play(1, 2, player_X);
    assert(!win);
    assert(!board.win_check(player_X));

    win = board.play(0, 1, player_O);
    assert(!win);
    assert(!board.win_check(player_O));

    win = board.play(1, 0, player_X);
    assert(win);
    assert(board.win_check(player_X));

    win = board.play(0, 2, player_O);
    assert(!win);
    assert(!board.win_check(player_O));
}

void test_hexboard_win_3(void)
{
    cout << __func__ << endl;

    bool win = false;
    HexBoard board(2);

    win = board.play(0, 0, player_O);
    assert(!win);
    assert(!board.win_check(player_O));

    win = board.play(1, 0, player_X);
    assert(!win);
    assert(!board.win_check(player_X));

    win = board.play(0, 1, player_O);
    assert(!win);
    assert(!board.win_check(player_O));

    win = board.play(1, 1, player_X);
    assert(win);
    assert(board.win_check(player_X));
}

void test_hexboard_occupied_save_restore()
{
    cout << __func__ << endl;
    HexBoard board(3);
    board.play(0, 2, player_O);
    board.play(1, 1, player_O);

    vector< vector<Player> > occupied_backup;
    board.occupied_save(occupied_backup);
    board.play(2, 0, player_O);
    assert(board.win_check(player_O));

    board.occupied_restore(occupied_backup);
    assert(!board.win_check(player_O));
    board.play(2, 1, player_O);
    assert(board.win_check(player_O));
}
