/*------------------------------------------------------------------------------
hex.cpp
Run a game of hex.
------------------------------------------------------------------------------*/
#include <cstdlib>  // atoi
#include <iostream>
#include <utility>  // pair

#include "hexgame.hpp"
#include "moveeval.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    const int max_width = 26u;

    if ((argc != 2) || (atoi(argv[1]) > max_width))  {
        cout << "Usage: hex n" << endl;
        cout << "    n: the width of the board, less than " << max_width
             << endl;
        exit(1);
    }

    HexGame game(atoi(argv[1]));

    game.start_prompt();
    game.player_setup_prompt_and_set();

    bool win;

    do {
        win = game.next_prompt_and_play();
        //MoveEvaluator ai(game, 100);
    } while (!win);

    game.winner_print();
}

