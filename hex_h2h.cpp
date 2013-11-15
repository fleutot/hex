/*------------------------------------------------------------------------------
hex.cpp
Run a game of hex.
------------------------------------------------------------------------------*/
#include <cstdlib>
#include <iostream>
#include <utility>  // pair

#include "hexboard.hpp"

using namespace std;

static void prompt(HexBoard& board, const Player player);
static bool input_get(const Player player, const HexBoard& board,
                      pair<unsigned, unsigned>& move);
static void win_display(const Player player);
static void switch_player(Player& player);

int main(int argc, char *argv[])
{
    const int max_width = 26;

    if ((argc != 2) || (atoi(argv[1]) > max_width)) {
        cout << "Usage: hex_h2h n" << endl;
        cout << "    n: the width of the board, less than "
             << max_width << endl;
        exit(1);
    }

    cout << "+---------------------------+" << endl;
    cout << "| Hex game, human vs human  |" << endl;
    cout << "| End the game with ctrl+c. |" << endl;
    cout << "+---------------------------+" << endl;
    cout << endl;

    HexBoard hex(atoi(argv[1]));
    Player player = Player::O;
    bool win = false;

    while (!win) {
        prompt(hex, player);

        pair<unsigned, unsigned> move;
        bool valid_move = input_get(player, hex, move);

        if (valid_move) {
            win = hex.play(move.first, move.second, player);
            if (win) {
                win_display(player);
            } else {
                switch_player(player);
            }
        }
    }
}

static void prompt(HexBoard& board, const Player player)
{
    cout << "=========================" << endl;
    cout << board << endl << endl;
    cout << "X plays north and south." << endl;
    cout << "O plays west and east." << endl;
    cout << endl;
    cout << "Player " << player2char[static_cast<short>(player)]
         << ", please enter your move: ";
}

// Get the input pair in 0 based integers, return false if there was an error.
static bool input_get(const Player player, const HexBoard& board,
                      pair<unsigned, unsigned>& move)
{
    string in;
    cin >> in;
    unsigned col = toupper(in[0]) - 'A';

    // Make the first char of the string a leading 0 (as suggested by the
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

// Use to display that player has won the game.
static void win_display(const Player player)
{
    cout << endl;
    cout << "Player " << player2char[static_cast<short>(player)] << " wins!"
         << endl;
}

// Switch from a player to the other.
static void switch_player(Player& player)
{
    if (player == Player::O) {
        player = Player::X;
    } else if (player == Player::X) {
        player = Player::O;
    } else {
        cerr << __func__ << ": error in player input." << endl;
        exit(1);
    }
}
