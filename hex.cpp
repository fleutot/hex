/*------------------------------------------------------------------------------
  hex.cpp
  Run a game of hex either:
- in a multiplayer environment, with API by Alexandre
  Kharlamov, https://github.com/alexkh/hexai/blob/master/dummy.cpp
  Big chunks of the code below are taken from the repository above.
- interactively, AI vs AI, Human vs Human, AI vs Human.
  ------------------------------------------------------------------------------*/
#include <chrono>
#include <climits>
#include <cstdlib>  // atoi, rand
#include <ctime>
#include <iostream>
#include <sstream>
#include <utility>  // pair

#include "hexgame.hpp"
#include "moveeval.hpp"

using namespace std;

static const unsigned max_size = 16u;

static void usage_print()
{
    cout << "Usage:" << endl;
    cout << "hex size" << endl;
    cout << "- interactive play" << endl;
    cout << "    size: size of the board side, less than " << max_size << endl;
    cout << endl;
    cout << "hex P [size] [iter]" << endl;
    cout << "- automatic play" << endl;
    cout << "    P: the player color (X or O). X plays first, north/south."
         << endl;
    cout << "    size: size of the board side, less than " << max_size << endl;
    cout << "    iter: max number of Monte-Carlo iterations per test move."
         << endl;
}

static void interactive_game(const unsigned size)
{
    HexGame game(size);
    cout << size <<endl;
    game.start_prompt();
    game.player_setup_prompt_and_set();

    bool win;

    do {
        win = game.next_prompt_and_play();
    } while (!win);

    game.winner_print();
}

static void automatic_game(const unsigned size, const char color,
                           const unsigned iter)
{
    HexGame game(size, iter);

    int result = game.autoplay_handshake(color);
    if (result < 0) {
        exit(result);
    }

    game.player_autoplay_setup(color);

    bool game_over;

    do {
        game_over = game.autoplay_next_move_play();
    } while (!game_over);
}

int main(int argc, char *argv[])
{
    // --- 8< -----
    // partly taken from https://github.com/alexkh/hexai/blob/master/dummy.cpp
    char color = 'X'; // can be X or O
    unsigned short board_side = 11; // side of the board minimum 3
    size_t iter = 1000; // number of iterations should be selectable

    // parse command line parameters
    argc = argc > 4? 4: argc; // forward compatibility measure
    switch(argc) {
    case 4:
    {
        stringstream ss; // used for reading numbers from strings
        ss << argv[3]; // put third argument into the stringstream
        ss >> iter; // read its numeric value
    }
    case 3:
    {
        stringstream ss;
        ss << argv[2];
        ss >> board_side;
        if (board_side > max_size) {
            cerr << color << " E: max board size is " << max_size << endl;
            return -2;
        }
        board_side = board_side < 3? 3: board_side; // minimum 3
    }
    case 2:
    {
        color = argv[1][0];
        if (color == 'X' || color == 'O') {
            automatic_game(board_side, color, iter);
        } else if (static_cast<unsigned>(atoi(argv[1])) <= max_size) {
            // More input checking would be nice.
            board_side = static_cast<unsigned>(atoi(argv[1]));
            interactive_game(board_side);
        } else {
            usage_print();
            return -1; // there is some error
        }
    }
    break;
    default:
        usage_print();
        break;
    }
    // --- >8 -----
    return 0;
}

