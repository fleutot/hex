/*------------------------------------------------------------------------------
Player class implementation
player.hpp
------------------------------------------------------------------------------*/
#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <iostream>

using namespace std;

enum class player_e {
    NONE,
    X,
    O   // This is capital letter o.
};

class Player {
public:
    Player(const player_e p = player_e::NONE): current(p) {}

    void set(const player_e p) {
        current = p;
    }

    player_e get() const {
        return current;
    }

    player_e other() const {
        if (current == player_e::O) {
            return player_e::X;
        } else if (current == player_e::X) {
            return player_e::O;
        } else {
            cerr << __func__ << ": erroneous value of current player." << endl;
            exit(1);
        }
    }

    bool is_player() const {
        return current != player_e::NONE;
    }

    void swap() {
        current = other();
    }

    friend std::ostream& operator<<(std::ostream& os, const Player& player);

private:
    player_e current;
};

std::ostream& operator<<(std::ostream& os, const Player& player);

#endif // PLAYER_HPP_INCLUDED
