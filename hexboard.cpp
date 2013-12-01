/*------------------------------------------------------------------------------
Implementation of a hex board
------------------------------------------------------------------------------*/
#include <algorithm>
#include <cctype>
#include <iostream>
#include <iomanip>

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

    // Add virtual nodes to the players' trees.
    trees_X.resize(2);
    trees_X[0].push_back(north);
    trees_X[1].push_back(south);

    trees_O.resize(2);
    trees_O[0].push_back(west);
    trees_O[1].push_back(east);

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

    // order of row and col here inverted, occupied_map is a vector of rows.
    occupied_map[row][col] = player;
    auto it = find(unoccupied_list.begin(), unoccupied_list.end(),
                   make_pair(col, row));
    unoccupied_list.erase(it);

    player_select(player);
    unsigned new_tree_index = update_trees(col, row);
    return connected_in_tree_check(side_a, side_b, new_tree_index);
}

void HexBoard::player_select(const Player player)
{
    if (player.get() == player_e::O) {
        trees = &trees_O;
        side_a = west;
        side_b = east;
    } else if (player.get() == player_e::X) {
        trees = &trees_X;
        side_a = north;
        side_b = south;
    } else {
        cerr << __func__ << ": undefined player." << endl;
        exit(1);
    }
}

unsigned HexBoard::update_trees(const unsigned col, const unsigned row)
{
    unsigned vertex_name = coord2lin(col, row);

    // Add the node as a new tree.
    unsigned new_tree_index = trees->size();
    trees->resize(new_tree_index + 1);
    (*trees)[new_tree_index].push_back(vertex_name);

    // Check if neighbors are already in trees, and merge in that case.
    vector<int> neighbors = board->neighbors_get(vertex_name);
    for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {
        unsigned neighbor_tree_index;
        if (containing_tree_get(*it, neighbor_tree_index)) {
            new_tree_index = trees_merge(neighbor_tree_index, new_tree_index);
        }
    }
    return new_tree_index;
}

bool HexBoard::containing_tree_get(const int vertex_name,
                                   unsigned& found_tree_index)
{
    for (unsigned i = 0; i < trees->size(); ++i) {
        auto found_it = find((*trees)[i].begin(), (*trees)[i].end(),
                             vertex_name);
        if (found_it != (*trees)[i].end()) {
            found_tree_index = i;
            return true;
        }
    }
    return false;
}

// Merge a and b together, return the index of the merged tree.
unsigned HexBoard::trees_merge(unsigned index_a, unsigned index_b)
{
    if (index_a == index_b) {
        return index_a;
    }

    vector< vector<int> >& forest = *trees;  // for readability.

    // Make sure to merge the shorter tree into the longer one. This function
    // was profiled to take much time, hence this optimization.
    if (forest[index_a].size() < forest[index_b].size()) {
        unsigned temp = index_a;
        index_a = index_b;
        index_b = temp;
    }

    forest[index_a].reserve(forest[index_a].size() + forest[index_b].size());
    forest[index_a].insert(forest[index_a].end(),
                           forest[index_b].begin(), forest[index_b].end());

    forest.erase(forest.begin() + index_b);
    if (index_a > index_b) {
        // The erased tree was before index_a, update the index.
        --index_a;
    }
    return index_a;
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
