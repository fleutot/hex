/*------------------------------------------------------------------------------
Hex game class implementation
------------------------------------------------------------------------------*/
#include <chrono>
#include <iostream>
#include <utility> // pair

#include "hexboard.hpp"
#include "hexgame.hpp"
#include "moveeval.hpp"

using namespace std;

void HexGame::start_prompt()
{
    cout << "+---------------------------+" << endl;
    cout << "|        Hex game           |" << endl;
    cout << "| End the game with ctrl+c. |" << endl;
    cout << "+---------------------------+" << endl;
    cout << endl;
}

void HexGame::player_setup_prompt_and_set()
{
    cout << "=========================" << endl;
    cout << "Setup:" << endl;

    bool entry_ok;
    do {
        cout << "Player X (plays first): (A)I or (H)uman?: ";
        string type;
        cin >> type;
        // This is not very clean, but suffice for now.
        entry_ok = char_to_player_type_set(type[0], player_X_type);
    } while (!entry_ok);

    do {
        cout << "Player O: (A)I or (H)uman?: ";
        string type;
        cin >> type;
        entry_ok = char_to_player_type_set(type[0], player_O_type);
    } while (!entry_ok);
}

bool HexGame::next_prompt_and_play()
{
    cout << "=========================" << endl;
    cout << board << endl << endl;
    cout << "X plays north and south." << endl;
    cout << "O plays west and east." << endl;
    cout << endl;

    pair<unsigned, unsigned> move;
    bool valid_move;

    do {
        cout << "Player " << current_player << ", please enter your move: ";

        if (current_player_type_get() == PlayerType::HUMAN) {
            valid_move = human_input_get(move);
        } else if (current_player_type_get() == PlayerType::AI) {
            valid_move = ai_input_get(move);
        }
    } while (!valid_move);

    bool win = board.play(move, current_player);

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

PlayerType HexGame::current_player_type_get()
{
    if (current_player.get() == player_e::X) {
        return player_X_type;
    } else if (current_player.get() == player_e::O) {
        return player_O_type;
    } else {
        cerr << __func__ << ": error in player type." << endl;
        exit(1);
    }
}

// Get the input pair in 0 based integers, return false if there was an error.
bool HexGame::human_input_get(pair<unsigned, unsigned>& move)
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
        cout << "############ Unauthorized move";
        move_print(make_pair(col, row));
        cout << ", try again! ###########" << endl;
        return false;
    }

    move.first = col;
    move.second = row;
    return true;
}

bool HexGame::ai_input_get(pair<unsigned, unsigned>& move)
{
    cout << "thinking... " << flush;

    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    MoveEvaluator evaluator(board, current_player);
    move = evaluator.best_move_calculate();
    cout << " -> ";
    move_print(move);
    cout << endl;

    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    cout << "Elapsed time: " << elapsed_seconds.count() << " s" << endl;

    return true;    // The AI only gives valid moves.
}

bool HexGame::char_to_player_type_set(const char c, PlayerType& type)
{
    if (toupper(c) == 'H') {
        type = PlayerType::HUMAN;
        return true;
    } else if (toupper(c) == 'A') {
        type = PlayerType::AI;
        return true;
    } else {
        return false;
    }
}
