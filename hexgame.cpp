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

void HexGame::player_autoplay_setup(const char color)
{
    if (color == 'X') {
        player_X_type = PlayerType::AI;
        player_O_type = PlayerType::OPPONENT_AI;
    } else {
        player_O_type = PlayerType::AI;
        player_X_type = PlayerType::OPPONENT_AI;
    }
}

int HexGame::autoplay_handshake(const char color)
{
    // send handshake message color: name of program by author
    // this string should uniquely identify the player
    cout << color
         << ": Hexjakt by Gauthier Östervall, https://github.com/fleutot/hex"
         << endl;

    if(color == 'X') {
        char c;
        // wait for other player's handshake message
        cin >> c; // should be the other player's color
        if(c != 'O') {
            cerr << "X. E: expecting handshake message from O" << endl;
            return -2;
        }
        cin >> c; // should be ':'
        if(c != ':') {
            cerr << "X. E: expecting : after O in handshake message" << endl;
            return -3;
        }
        // ignore the rest of the line
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return 0;
}

bool HexGame::autoplay_next_move_play()
{
    pair<unsigned, unsigned> move;
    int error_code = 0;
    double elapsed_milli = 0.0;
    bool opponent_give_up = false;

    if (current_player_type_get() == PlayerType::AI) {
        error_code = autoplay_input_move_make(move, elapsed_milli);
    } else if (current_player_type_get() == PlayerType::OPPONENT_AI) {
        error_code = autoplay_opponent_move_read(move, opponent_give_up);
    }

    if (error_code < 0) {
        exit(error_code);
    }

    if (opponent_give_up) {
        // game over.
        return true;
    }

    bool win = board.play(move, current_player);

    if (current_player_type_get() == PlayerType::AI) {
        autoplay_move_print(move, win, elapsed_milli);
    }

    if (win) {
        winner = current_player;
    } else {
        current_player.swap();
    }
    return win;
}

void HexGame::autoplay_move_print(pair<unsigned, unsigned> const& move,
                                  const bool win, const double elapsed_milli)
{
    static unsigned move_count = 0;
    // Output for this new move.
    cout << current_player;
    move_print(move);
    if (win) {
        cout << ".";
    } else {
        cout << " ";
    }
    cout << "#" << ++move_count << " ";
    cout << "t=" << elapsed_milli << "ms" << endl;
}

void HexGame::autoplay_capitulate_print()
{
    cout << current_player << "." << endl;
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

    MoveEvaluator evaluator(board, current_player, max_simulations,
                            simulations_per_test_move);
    move = evaluator.best_move_calculate();
    cout << " -> ";
    move_print(move);
    cout << endl;

    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    cout << "Elapsed time: " << elapsed_seconds.count() << " s" << endl;

    return true;    // The AI only gives valid moves.
}

int HexGame::autoplay_input_move_make(pair<unsigned, unsigned>& move,
                                      double& milliseconds)
{
    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    MoveEvaluator evaluator(board, current_player, max_simulations,
                            simulations_per_test_move);
    move = evaluator.best_move_calculate();

    end = chrono::system_clock::now();
    chrono::duration<double, milli> elapsed_milliseconds = end - start;
    milliseconds = elapsed_milliseconds.count();

    return 0;    // Assume the AI only gives valid moves.
}

int HexGame::autoplay_opponent_move_read(pair<unsigned, unsigned>& move,
                                         bool& give_up)
{
    give_up = false;
    char column;
    unsigned col_n, row_n;  // zero-based indeces/
    char color;

    cin >> color;
    cin >> column; // lower case letter representing board column. May be dot if
                   // the player wants to quit.
    if (color != 'O' && color != 'X') {
        return -6;
    }
    if (column == ':') {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return 0;   // return with no error.
    }
    if(column == '.') {
        give_up = true;
        return 0;
    }
    col_n = tolower(column) - 'a';
    if(col_n >= board.size_get()) {
        cerr << color << ". E: " << color <<
            " received illegal column: '" << column << "'\n";
        return -4;
    }
    cin >> row_n;
    if(row_n > board.size_get()) {
        cerr << color << ". E: " << color <<
            " received illegal row: '" << row_n << "'\n";
        return -5;
    }

    move = make_pair(col_n, row_n - 1);

    char c = cin.peek();
    if(c == '.') { // dot at the end of the other player's move
        // means that he wins, or maybe he gives up - game over
        return 0;
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return 0;
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
