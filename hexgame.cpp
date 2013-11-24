/*------------------------------------------------------------------------------
Hex game class implementation
------------------------------------------------------------------------------*/

#include <iostream>
#include <utility> // pair

#include "hexboard.hpp"
#include "hexgame.hpp"

using namespace std;

void HexGame::start_prompt()
{
    cout << "+---------------------------+" << endl;
    cout << "|        Hex game           |" << endl;
    cout << "| End the game with ctrl+c. |" << endl;
    cout << "+---------------------------+" << endl;
    cout << endl;
}

void HexGame::player_setup_prompt()
{
}

bool HexGame::next_prompt_and_play()
{
    cout << "=========================" << endl;
    cout << board << endl << endl;
    cout << "X plays north and south." << endl;
    cout << "O plays west and east." << endl;
    cout << endl;
    cout << "Player " << current_player << ", please enter your move: ";

    pair<unsigned, unsigned> move;
    bool valid_move;

    do {
        valid_move = input_get(move);
    } while (!valid_move);

    bool win = board.play(move.first, move.second, current_player);

    if (win) {
        winner = current_player;
    } else {
        current_player.swap();
    }

    return win;
}

void HexGame::winner_print()
{
    cout << endl;
    cout << board << endl << endl;
    cout << "\t\t!!! Player " << winner << " wins !!!" << endl;
}

void HexGame::player_switch()
{
}

// Get the input pair in 0 based integers, return false if there was an error.
bool HexGame::input_get(pair<unsigned, unsigned>& move)
{
    string in;
    cin >> in;
    unsigned col = toupper(in[0]) - 'A';

    // Make the first char of the string a leading 0 (as suggested in the
    // forums).
    in[0] = '0';
    unsigned row = atoi(in.c_str()) - 1;

    if ((col >= board.size_get()) || (row >= board.size_get()) ||
        board.occupied_check(col, row)
        ) {
        cout << "############ Unauthorized move, try again! ###########" << endl;
        return false;
    }

    move.first = col;
    move.second = row;
    return true;
}
