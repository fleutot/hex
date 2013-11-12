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

int main(void)
{
    test_hexboard_constructor();
    test_hexboard_display();
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
    cout << board;
}
