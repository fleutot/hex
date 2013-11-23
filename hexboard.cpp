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

const char player2char[] = {
    [Player::NONE]  = '.',
    [Player::O]     = 'O',
    [Player::X]     = 'X'
};

// 4 extra virtual nodes for the board, representing the edges. These were added
// to ease checking for winning condition.
HexBoard::HexBoard(unsigned size): size(size), board(size * size + 4),
                                   west(size * size), east(size * size + 1),
                                   north(size * size + 2), south(size * size +3)
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

    // Virtual nodes connect to all the nodes of their board side.
    for (unsigned i = 0; i < size; ++i) {
        board.edge_add(west, coord2lin(0, i));
        board.edge_add(east, coord2lin(size - 1, i));
        board.edge_add(north, coord2lin(i, 0));
        board.edge_add(south, coord2lin(i, size - 1));
    }

    // Add virtual nodes to the players' trees.
    trees_X.resize(2);
    trees_X[0].push_back(north);
    trees_X[1].push_back(south);

    trees_O.resize(2);
    trees_O[0].push_back(west);
    trees_O[1].push_back(east);
}
    
HexBoard::~HexBoard()
{
    // do nothing. The build process wanted a definition here, rather than an
    // empty one in the class implementation, or implicit definition.  It would
    // be interesting to know why, but there is no time now.
}

bool HexBoard::sanity_check()
{
    return (board.nb_vertices_get() == (size * size + 4)
            && board.nb_edges_get() == (
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
        || (occupied_map[row][col] != Player::NONE) // vector of rows.
        ) {
        cout << "Unauthorized move." << endl;
        return false;
    }

    // order of row and col here inverted, occupied_map is a vector of rows.
    occupied_map[row][col] = player;

    player_select(player);
    unsigned new_tree_index = update_trees(col, row, player);
    return connected_in_tree_check(side_a, side_b, new_tree_index);
}

void HexBoard::player_select(const Player player)
{
    if (player == Player::O) {
        trees = &trees_O;
        side_a = east;
        side_b = west;
    } else if (player == Player::X) {
        trees = &trees_X;
        side_a = north;
        side_b = south;
    } else {
        cerr << __func__ << ": undefined player." << endl;
        exit(1);
    }
}

unsigned HexBoard::update_trees(const unsigned col, const unsigned row,
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
    return new_tree_index;
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

void HexBoard::trees_merge(unsigned& index_a, unsigned index_b)
{
    if (index_a == index_b) {
        return;
    }

    vector< vector<int> >& forest = *trees;  // for readability.

    forest[index_a].reserve(forest[index_a].size() + forest[index_b].size());
    forest[index_a].insert(forest[index_a].end(),
                           forest[index_b].begin(), forest[index_b].end());

    forest.erase(forest.begin() + index_b);
    if (index_a > index_b) {
        // A tree before index_a was erased, update the index.
        --index_a;
    }
}

bool HexBoard::connected_in_tree_check(const int node_a, const int node_b,
                                       const unsigned tree_index)
{
    bool a_found = false;
    bool b_found = false;
    for (auto vertex: (*trees)[tree_index]) {
        if (node_a == vertex) {
            a_found = true;
        } else if (node_b == vertex) {
            b_found = true;
        }
    }
    return a_found && b_found;
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
            os << setw(slot_width - 2)
               << player2char[static_cast<short>(*col_it)];
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
