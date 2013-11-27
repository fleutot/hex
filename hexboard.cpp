/*------------------------------------------------------------------------------
Implementation of a hex board
------------------------------------------------------------------------------*/
#include <algorithm>
#include <cctype>
#include <iostream>
#include <iomanip>
#include <vector>

#include "hexboard.hpp"
#include "graph.hpp"
#include "player.hpp"

using namespace std;

const int nb_players = 2;

HexBoard::HexBoard(unsigned size): size(size),
                                   west(size * size), east(size * size + 1),
                                   north(size * size + 2), south(size * size +3)
{
    // 4 extra virtual nodes for the board, representing the rims. These were
    // added to ease checking for winning condition.
    board = make_shared<Graph>(size * size + 4);

    occupied_map.resize(size);
    for (auto it = occupied_map.begin(); it != occupied_map.end(); ++it) {
        it->resize(size);   // Default value for objects Player is NONE.
    }

    // Create the edges of the graph. Besides special cases around the sides of
    // the board, create three edges for each vertix: 3 o'clock, 5 o'clock, 7
    // o'clock. Special cases are the west, east, and south sides of the board.

    // West and east sides (not the south corners, which are done with south side):
    for (unsigned row = 0; row < size - 1; ++row) {
        // West edge
        unsigned vertix = coord2lin(0, row);
        unsigned vertix_3oclock = coord2lin(1, row);
        unsigned vertix_5oclock = coord2lin(0, row + 1);
        board->edge_add(vertix, vertix_3oclock);
        board->edge_add(vertix, vertix_5oclock);
        // no 7 o'clock for this side of the board.

        // East edge
        vertix = coord2lin(size - 1, row);
        vertix_5oclock = coord2lin(size - 1, row + 1);
        unsigned vertix_7oclock = coord2lin(size - 2, row + 1);
        board->edge_add(vertix, vertix_5oclock);
        board->edge_add(vertix, vertix_7oclock);
        // no 3 o'clock for this side of the board.
    }
    // South edge of the board.
    for (unsigned col = 0; col < size - 1; ++col) {
        // 3 o'clock edge.
        board->edge_add(coord2lin(col, size - 1), coord2lin(col + 1, size - 1));
    }
    // All other vertices.
    for (unsigned col = 1; col < size - 1; ++col) {
        for (unsigned row = 0; row < size - 1; ++row) {
            unsigned vertix = coord2lin(col, row);
            unsigned vertix_3oclock = coord2lin(col + 1, row);
            unsigned vertix_5oclock = coord2lin(col, row + 1);
            unsigned vertix_7oclock = coord2lin(col - 1, row + 1);
            board->edge_add(vertix, vertix_3oclock);
            board->edge_add(vertix, vertix_5oclock);
            board->edge_add(vertix, vertix_7oclock);
        }
    }

    // Virtual nodes connect to all the nodes of their board side.
    for (unsigned i = 0; i < size; ++i) {
        board->edge_add(west, coord2lin(0, i));
        board->edge_add(east, coord2lin(size - 1, i));
        board->edge_add(north, coord2lin(i, 0));
        board->edge_add(south, coord2lin(i, size - 1));
    }

    // All slots are unoccupied to start with.
    unoccupied_list.reserve(size * size);
    for (unsigned row = 0; row < size; ++row) {
        for (unsigned col = 0; col < size; ++col) {
            unoccupied_list.push_back(make_pair(col, row));
        }
    }
}

bool HexBoard::sanity_check()
{
    return (board->nb_vertices_get() == (size * size + 4)
            && board->nb_edges_get() == (
                (3 * size * (size - 1) - (size - 1))    // real board edges
                + (4 * size))    // edges to virtual nodes.
            && occupied_map.size() == size
            && occupied_map[0].size() == size
            && occupied_map[size - 1].size() == size
            );
}

bool HexBoard::play(unsigned col, unsigned row, Player player)
{
    if ((col > size - 1)
        || (row > size - 1)
        || (occupied_map[row][col].is_player()) // vector of rows.
        ) {
        cout << "Unauthorized move: (" << col << ", " << row << ") = " ;
        move_print(make_pair(col, row)) ;
        cout << endl;
        return false;
    }
    place(col, row, player);
    return win_check(player);
}

void HexBoard::place(const unsigned col, const unsigned row, const Player player)
{
    // order of row and col here inverted, occupied_map is a vector of rows.
    occupied_map[row][col] = player;
    auto it = find(unoccupied_list.begin(), unoccupied_list.end(),
                   make_pair(col, row));
    unoccupied_list.erase(it);
}

void HexBoard::fill_up(Player player)
{
    vector< pair<unsigned, unsigned> > free_pos = unoccupied_list_get();

    // Order the future moves randomly.
    random_shuffle(free_pos.begin(), free_pos.end());

    // If the number of free position is not even, first_player is the one to
    // play once more than the other. This is solved with integer division,
    // which truncates downwards, and starting with the other player.
    player.swap();
    unsigned i;
    for (i = 0; i < free_pos.size() / 2; ++i) {
        occupied_map[free_pos[i].second][free_pos[i].first] = player;
    }
    player.swap();
    for (; i < free_pos.size(); ++i) {
        occupied_map[free_pos[i].second][free_pos[i].first] = player;
    }
}

bool HexBoard::win_check(const Player player)
{
    player_select(player);

    vector<int> unvisited = occupied_list_get(player);
    unsigned node = side_a; // Start board rim.
    unvisited.push_back(side_b);    // Target board rim.
    return win_search_recursive(node, unvisited);
}

bool HexBoard::win_search_recursive(const int node, vector<int>& unvisited)
{
    vector<int> neighbors = board->neighbors_get(node);
    vector<int> player_neighbors;

    for (unsigned i = 0; i < unvisited.size(); ++i) {
        if (find(neighbors.begin(), neighbors.end(), unvisited[i])
            != neighbors.end()) {
            player_neighbors.push_back(unvisited[i]);
            if (unvisited[i] == side_b) {
                // The other side was reached.
                return true;
            }
            // This neighbor was visited, do not test is again.
            unvisited.erase(unvisited.begin() + i);
            // Since the current element was erased, keep looking here.
            --i;
            if (unvisited.size() == 0) {
                // No more nodes to reach.
                return false;
            }
        }
    }

    for (auto n: player_neighbors) {
        if (win_search_recursive(n, unvisited)) {
            return true;
        }
    }
    return false;
}

vector<int> HexBoard::occupied_list_get(const Player player)
{
    vector<int> player_nodes;
    for (unsigned row = 0; row < size; ++row) {
        for (unsigned col = 0; col < size; ++col) {
            if (occupied_map[row][col] == player) {
                player_nodes.push_back(coord2lin(col, row));
            }
        }
    }
    return player_nodes;
}

void HexBoard::player_select(const Player player)
{
    if (player.get() == player_e::O) {
        side_a = east;
        side_b = west;
    } else if (player.get() == player_e::X) {
        side_a = north;
        side_b = south;
    } else {
        cerr << __func__ << ": undefined player." << endl;
        exit(1);
    }
}

ostream& operator<< (ostream& os, const HexBoard& board)
{
    // Changing slot_width may need more changes for the display of the links.
    const int slot_width = 4;

    // column labels
    os << setw(slot_width / 2) << " ";
    for (unsigned i = 0; i < board.size; ++i) {
        os << setw(slot_width) << static_cast<char>(i + 'A');
    }
    os << endl;

    unsigned row_index = 0;
    for (auto row_it = board.occupied_map.begin();
         row_it != board.occupied_map.end();
         ++row_it
        ) {
        // offset to make the board slanted.
        for (unsigned i = 0; i < row_index; ++i) {
            os << setw(slot_width / 2) << " ";
        }
        ++row_index;
        os << setw(slot_width) << row_index; // row label

        // The row itself.
        for (auto col_it = row_it->begin(); col_it != row_it->end(); ++col_it) {
            os << setw(slot_width - 2) << *col_it;
            // Print the horizontal link after all but last column.
            if (col_it != row_it->end() - 1) {
                os << " -";
            }
        }
        os << setw(slot_width) << row_index << endl; // repeat the row label

        // Print the slanted links after all but last row.
        if (row_it != board.occupied_map.end() - 1) {
            board.slanted_links_row_print(os, slot_width, row_index);
        }
    }

    // repeat column labels.
    for (unsigned i = 0; i < row_index; ++i) {
        os << setw(slot_width / 2) << " ";
    }
    for (unsigned i = 0; i < board.size; ++i) {
        os << setw(slot_width) << static_cast<char>(i + 'A');
    }

    return os;
}

void HexBoard::slanted_links_row_print(ostream& os, const unsigned slot_width,
                                       const unsigned row_index) const
{
    // offset to make the row of links also slanted.
    for (unsigned i = 0; i < row_index + 1; ++i) {
        os << setw(slot_width / 2) << " ";
    }
    os << " ";  // Extra space to align in a slanted fashion to the slot above.

    // Minus one, the last column is a special case.
    for (unsigned i = 0; i < size - 1; ++i) {
        os << setw(slot_width) << " \\ /";
    }
    os << " \\" << endl;
}

void move_print(const pair<unsigned, unsigned> move)
{
    cout << static_cast<char>(move.first + 'A') << move.second + 1;
}
