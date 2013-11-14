/*------------------------------------------------------------------------------
Implementation of a hex board
------------------------------------------------------------------------------*/
#include <cctype>
#include <iostream>
#include <iomanip>

#include "hexboard.hpp"
#include "graph.hpp"

using namespace std;

const int nb_players = 2;

static const char player2char[] = {
    [Player::NONE]  = '.',
    [Player::O]     = 'O',
    [Player::X]     = 'X'
};

HexBoard::HexBoard(unsigned size): size(size), board(size * size)
{
    occupied_map.resize(size);
    for (auto it = occupied_map.begin(); it != occupied_map.end(); ++it) {
        it->resize(size, Player::NONE);
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
        board.edge_add(vertix, vertix_3oclock);
        board.edge_add(vertix, vertix_5oclock);
        // no 7 o'clock for this side of the board.

        // East edge
        vertix = coord2lin(size - 1, row);
        vertix_5oclock = coord2lin(size - 1, row + 1);
        unsigned vertix_7oclock = coord2lin(size - 2, row + 1);
        board.edge_add(vertix, vertix_5oclock);
        board.edge_add(vertix, vertix_7oclock);
        // no 3 o'clock for this side of the board.
    }
    // South edge of the board.
    for (unsigned col = 0; col < size - 1; ++col) {
        // 3 o'clock edge.
        board.edge_add(coord2lin(col, size - 1), coord2lin(col + 1, size - 1));
    }
    // All other vertices.
    for (unsigned col = 1; col < size - 1; ++col) {
        for (unsigned row = 0; row < size - 1; ++row) {
            unsigned vertix = coord2lin(col, row);
            unsigned vertix_3oclock = coord2lin(col + 1, row);
            unsigned vertix_5oclock = coord2lin(col, row + 1);
            unsigned vertix_7oclock = coord2lin(col - 1, row + 1);
            board.edge_add(vertix, vertix_3oclock);
            board.edge_add(vertix, vertix_5oclock);
            board.edge_add(vertix, vertix_7oclock);
        }
    }
}
    
HexBoard::~HexBoard()
{
    // do nothing. The build process wanted a definition here, rather than an
    // empty one in the class implementation, or implicit definition.  It would
    // be interesting to know why, but there is no time now.
}

bool HexBoard::sanity_check()
{
    return (board.nb_vertices_get() == (size * size)
            && board.nb_edges_get() == (3 * size * (size - 1) - (size - 1))
            && occupied_map.size() == size
            && occupied_map[0].size() == size
            && occupied_map[size - 1].size() == size
        );
}

bool HexBoard::play(unsigned col, unsigned row, Player player)
{
    if ((col > size - 1)
        || (row > size - 1)
        || (occupied_map[col][row] != Player::NONE)
        ) {
        cout << "Unauthorized move." << endl;
        return false;
    }

    player_select(player);

    // order of row and col here inverted, occupied_map is a vector of rows.
    occupied_map[row][col] = player;
    update_trees(col, row, player);
    return true;
}

void HexBoard::player_select(const Player player)
{
    if (player == Player::O) {
        trees = &trees_O;
    } else if (player == Player::X) {
        trees = &trees_X;
    } else {
        cerr << __func__ << ": undefined player." << endl;
        exit(1);
    }
}

void HexBoard::update_trees(const unsigned col, const unsigned row,
                            const Player player)
{
    unsigned vertex_name = coord2lin(col, row);

    // Add the node as a new tree.
    unsigned new_tree_index = trees->size();
    trees->resize(new_tree_index + 1);
    (*trees)[new_tree_index].push_back(vertex_name);

    // Check if neighbors are already trees, and merge in that case.
    vector<int> neighbors = board.neighbors_get(vertex_name);
    for (auto vertex_name: neighbors) {
        unsigned neighbor_tree_index;
        if (containing_tree_get(vertex_name, neighbor_tree_index)) {
            trees_merge(neighbor_tree_index, new_tree_index);
            // The tree that was earlier created with only one vertex does not
            // exist anymore, it has been merged with adjacent_tree.
            // Further merges must happen with the old but updated tree.
            new_tree_index = neighbor_tree_index;
        }
    }
}

bool HexBoard::containing_tree_get(const int vertex_name, unsigned& found_tree_index)
{
    for (unsigned i = 0; i < trees->size(); ++i) {
        for (auto test_vertex: (*trees)[i]) {
            if (test_vertex == vertex_name) {
                found_tree_index = i;
                return true;
            }
        }
    }
    return false;
}

void HexBoard::trees_merge(unsigned index_a, unsigned index_b)
{
    if (index_a == index_b) {
        return;
    }

    vector< vector<int> >& forest = *trees;  // for readability.

    forest[index_a].reserve(forest[index_a].size() + forest[index_b].size());
    forest[index_a].insert(forest[index_a].end(),
                           forest[index_b].begin(), forest[index_b].end());
    forest.erase(forest.begin() + index_b);
}

ostream& operator<< (ostream& os, HexBoard& board)
{
    const int slot_width = 4;

    // column labels
    os << setw(slot_width) << " ";
    for (unsigned i = 0; i < board.size; ++i) {
        os << setw(slot_width) << static_cast<char>(i + 'A');
    }
    os << endl;

    unsigned row_index = 0;
    for (auto row: board.occupied_map) {
        // offset to make the board slanted.
        for (unsigned i = 0; i < row_index; ++i) {
            os << setw(slot_width / 2) << " ";
        }
        os << setw(slot_width) << ++row_index; // row label
        for (auto slot_value: row) {
            os << setw(slot_width) << player2char[static_cast<short>(slot_value)];
        }
        os << setw(slot_width) << row_index << endl; // repeat the row label
    }

    // repeat column labels. One extra half slot to align with the previous row.
    for (unsigned i = 0; i < row_index + 1; ++i) {
        os << setw(slot_width / 2) << " ";
    }
    for (unsigned i = 0; i < board.size; ++i) {
        os << setw(slot_width) << static_cast<char>(i + 'A');
    }

    return os;
}
