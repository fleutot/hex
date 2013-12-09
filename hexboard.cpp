/*------------------------------------------------------------------------------
Implementation of a hex board
------------------------------------------------------------------------------*/
#include <algorithm>
#include <ctime>
#include <cctype>
#include <iostream>
#include <iomanip>
#include <vector>

#include "hexboard.hpp"
#include "graph.hpp"
#include "player.hpp"

using namespace std;

const int nb_players = 2;

static inline unsigned unsigned_div_ceil(const unsigned num,
                                         const unsigned denom);

static inline void comb_step_down(vector<uint16_t>& combed,
                                  vector<uint16_t> const& occupied,
                                  const unsigned i);

static inline bool comb_step_up(vector<uint16_t>& combed,
                                vector<uint16_t> const& occupied,
                                const unsigned i);

static inline bool comb_laterally_spread(vector<uint16_t>& combed,
                                         vector<uint16_t> const& occupied,
                                         const unsigned i);


// 4 extra virtual nodes for the board, representing the edges. These were added
// to ease checking for winning condition.
HexBoard::HexBoard(unsigned size): size(size),
                                   west(size * size), east(size * size + 1),
                                   north(size * size + 2), south(size * size +3)
{
    // Seed only once, at object creation. This could lead to troubles if
    // several boards were to be created within a second. Not a problem now.
    random_engine.seed(time(0));

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
    occupied_O.resize(size);
    occupied_X.resize(size);
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
    occupied_set(col, row, player);
}

void HexBoard::unplace(const unsigned col, const unsigned row)
{
    Player old_player = occupied_map[row][col];
    occupied_reset(col, row, old_player);
}

//  ----------------------------------------------------------------------------
/// \brief  Fill up the rest of the board randomly.
/// \param  The first player to place a stone.
//  ----------------------------------------------------------------------------
void HexBoard::fill_up(Player player)
{
    vector< pair<unsigned, unsigned> >& free_pos = unoccupied_list_get();

    // Order the future moves randomly.
    shuffle(free_pos.begin(), free_pos.end(), random_engine);

    // If the number of free position is not even, first_player is the one to
    // play once more than the other. This is solved with integer division,
    // (which truncates downwards if not even) and starting with the other
    // player.
    player.swap();
    unsigned i;
    for (i = 0; i < free_pos.size() / 2; ++i) {
        occupied_map[free_pos[i].second][free_pos[i].first] = player;
    }
    player.swap();
    for (; i < free_pos.size(); ++i) {
        occupied_map[free_pos[i].second][free_pos[i].first] = player;
    }
    unoccupied_list.clear();
}

//  ----------------------------------------------------------------------------
/// \brief  Fill up the half the board randomly with one player's stones, and
/// check if that player won. The resulting board is not one that could be
/// reached in a real game, so this hexboard is not very usable after this
/// function. win_check() for example may not be used directly.
/// \param  The player that would be the next player.
/// \return Win for that player.
//  ----------------------------------------------------------------------------
bool HexBoard::fill_up_half_and_win_check(Player player)
{
    vector< pair<unsigned, unsigned> >& free_pos = unoccupied_list_get();
    // Order the future moves randomly.
    shuffle(free_pos.begin(), free_pos.end(), random_engine);

    // If the number of free positions is not even, player is the one to
    // play once more than the other. Round up the integer division.
    player_select(player);
    for (unsigned i = 0; i < unsigned_div_ceil(free_pos.size(), 2); ++i) {
        (this->*occupied_player_set)(free_pos[i].first, free_pos[i].second);
    }

    return win_check(player);
}

bool HexBoard::win_check(const Player player)
{
    vector<uint16_t>& occupied = (player.get() == player_e::O) ?
        occupied_O :
        occupied_X;

    vector<uint16_t> combed(size);

    // Was the current row modified by a go-back-up operation?
    bool previous_modified = false;

    combed[0] = occupied[0];
    unsigned i = 0;
    while ((combed[i] != 0) && (i < size)) {
        // Connections 3 and 9 o'clock on this row.
        // Nothing happens at the first row since combed[0] is initialized as
        // occupied[0].
        bool lateral_spread = comb_laterally_spread(combed, occupied, i);

        if (lateral_spread || previous_modified) {
            // Connections 11 and 1 o'clock to previous row.
            // Test a step upwards if:
            // - there was lateral spread, since it could connect to a new node
            // on the previous row,
            // - the current row was modified while going upwards on the
            // previous step, since there could be a new diagonal connection.
            // The first row cannot be modified, because of its initialization.
            previous_modified = comb_step_up(combed, occupied, i);
            if (previous_modified) {
                --i;    // keep going upwards.
            } else {
                // Nothing new, go on.
                comb_step_down(combed, occupied, i);
                ++i;
            }
        } else {
            // Connections 5 and 7 o'clock from this row.
            comb_step_down(combed, occupied, i);
            ++i;
        }
    }
    return combed[size - 1] != 0;
}


static inline void comb_step_down(vector<uint16_t>& combed,
                                  vector<uint16_t> const& occupied,
                                  const unsigned i)
{
    combed[i + 1] |= combed[i] | (combed[i] >> 1u);
    combed[i + 1] &= occupied[i + 1];
}

static inline bool comb_step_up(vector<uint16_t>& combed,
                                vector<uint16_t> const& occupied,
                                const unsigned i)
{
    uint16_t old_previous_row = combed[i - 1];
    combed[i - 1] |= combed[i] | (combed[i] << 1u);
    combed[i - 1] &= occupied[i - 1];
    return old_previous_row != combed[i - 1];
}

static inline bool comb_laterally_spread(vector<uint16_t>& combed,
                                         vector<uint16_t> const& occupied,
                                         const unsigned i)
{
    uint16_t old_combed = combed[i];    // To see if there was actual spread.
    uint16_t tmp_combed;
    do {
        tmp_combed = combed[i];
        combed[i] |= (combed[i] << 1u) & occupied[i];
        combed[i] |= (combed[i] >> 1u) & occupied[i];
    } while (combed[i] != tmp_combed);
    return old_combed != combed[i];
}



void HexBoard::occupied_list_get(const Player player, vector<int>& list)
{
    vector<int> player_nodes;
    for (unsigned row = 0; row < size; ++row) {
        for (unsigned col = 0; col < size; ++col) {
            if (occupied_map[row][col] == player) {
                list.push_back(coord2lin(col, row));
            }
        }
    }
}

inline void HexBoard::player_select(const Player player)
{
    if (player.get() == player_e::O) {
        HexBoard::occupied_player_set = &HexBoard::occupied_O_set;
    } else if (player.get() == player_e::X) {
        HexBoard::occupied_player_set = &HexBoard::occupied_X_set;
    } else {
        cerr << __func__ << ": undefined player." << endl;
        exit(1);
    }
}

void HexBoard::occupied_set(unsigned col, unsigned row, Player player,
                            int value)
{
    if (value > 0) {
        // order of row and col here inverted, occupied_map is a vector of rows.
        occupied_map[row][col] = player;
        auto it = find(unoccupied_list.begin(), unoccupied_list.end(),
                       make_pair(col, row));
        unoccupied_list.erase(it);
        if (player.get() == player_e::X) {
            occupied_X[row] |= (1 << col);
        } else if (player.get() == player_e::O) {
            // col and row are inverted for common win_check().
            occupied_O[col] |= (1 << row);
        }

    } else {
        occupied_map[row][col] = player_e::NONE;
        unoccupied_list.push_back(make_pair(col, row));
        if (player.get() == player_e::X) {
            occupied_X[row] &= ~(1 << col);
        } else if (player.get() == player_e::O) {
            // col and row are inverted for common win_check().
            occupied_O[col] &= ~(1 << row);
        }
    }
}

inline void HexBoard::occupied_X_set(unsigned col, unsigned row)
{
    occupied_X[row] |= (1 << col);
}

inline void HexBoard::occupied_O_set(unsigned col, unsigned row)
{
    occupied_O[col] |= (1 << row);
}

// -----------------------------------------------------------------------------
// Print operations
// -----------------------------------------------------------------------------

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

// -----------------------------------------------------------------------------
// Local functions
// -----------------------------------------------------------------------------

static inline unsigned unsigned_div_ceil(const unsigned num,
                                         const unsigned denom)
{
    return (num + denom - 1) / denom;
}
