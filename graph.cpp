/*----------------------------------------------------------------------------
Graph class implementation
----------------------------------------------------------------------------*/
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>  // For width of display
#include <cstdlib>
#include <vector>

#include "graph.hpp"

using namespace std;

const cost_t COST_MAX = numeric_limits<cost_t>::max();

//  ----------------------------------------------------------------------------
/// \brief  Pick a number of elements of a vector randomly.
/// \param  input   The vector that contains data to pick from.
/// \param  nb_out  The number of elements to randomly pick.
/// \return A new vector containing nb_out elements, chosen randomly form input.
//  ----------------------------------------------------------------------------
template <class T>
vector<T> random_pick(vector<T> input, const int nb_out)
{
    vector<T> out;
    out.reserve(nb_out);

    for (int i = 0; i < nb_out; ++i) {
        int index = rand() % input.size();
        // Put the newly picked element in the output vector.
        out.push_back(input[index]);
        // Remove the newly picked element to avoid picking it twice.
        input.erase(input.begin() + index);
    }
    return out;
}

// ----------------------------------------------------------------------------
/// \brief Swap the values of the parameter if the first one is not less than
/// the second one.
//----------------------------------------------------------------------------
template <class T>
static void reorder_values(T& a, T& b)
{
    if (b < a) {
        T temp = b;
        b = a;
        a = temp;
    }
}

//  ----------------------------------------------------------------------------
/// Class Edge
//  ----------------------------------------------------------------------------
Edge::Edge(int start, int end, cost_t cost)
{
    if (start == end) {
        cerr << "Created an edge with same start and end! (" << start
             << ")" << endl;
    } else if (start > end) {
        // Edges are undirected, have always start < end as a convention.
        // This may speed up searches.
        this->start = end;
        this->end = start;
    } else {
        this->start = start;
        this->end = end;
    }
    this->cost = cost;
}

Edge::~Edge()
{
    // Nothing to do
}

int Edge::start_get(void) const
{
    return start;
}

int Edge::end_get(void) const
{
    return end;
}

cost_t Edge::cost_get(void) const
{
    return cost;
}

void Edge::cost_set(const cost_t cost)
{
    this->cost = cost;
}

// These operators are to recognize the edges' position, the cost they carry is
// irrelevant.
bool operator==(const Edge& a, const Edge& b)
{
    return ((a.start_get() == b.start_get()) && (a.end_get() == b.end_get()));
}

bool operator!=(const Edge& a, const Edge& b)
{
    return ((a.start_get() != b.start_get()) || (a.end_get() != b.end_get()));
}

ostream& operator<<(ostream& os, const Edge& e)
{
    os << "(" << e.start << ", " << e.end << "): cost "
       << e.cost;
    return os;
}

//  ----------------------------------------------------------------------------
/// \brief  Graph constructor
/// \param  nb_vertices Number of vertices (nodes) to create.
/// \param  edge_density    Fraction of the total number of possible edges,
/// which must be an actual edge.
/// \param  max_cost    Maximal cost for any of the edges created.
//  ----------------------------------------------------------------------------
Graph::Graph(const int nb_vertices = 50): nb_vertices(nb_vertices), nb_edges(0)
{
    node_names_init();
    neighbors.resize(nb_vertices);

    costs.resize(nb_vertices);
    for (int i = 0; i < nb_vertices; ++i) {
        costs[i].resize(nb_vertices);
    }
}

// Constructor with file input.
Graph::Graph(const string filename): nb_edges(0)
{
    ifstream ifp(filename);

    ifp >> nb_vertices;

    costs.resize(nb_vertices);
    for (int i = 0; i < nb_vertices; ++i) {
        costs[i].resize(nb_vertices);
    }
    node_names_init();
    neighbors.resize(nb_vertices);

    int start, end;
    cost_t cost;

    while (ifp >> start >> end >> cost) {
        edge_add(start, end, cost);
    }
}

//  ----------------------------------------------------------------------------
/// \brief  Destructor
//  ----------------------------------------------------------------------------
Graph::~Graph(void)
{
    // Nothing to deallocate.
}

//  ----------------------------------------------------------------------------
/// \brief  Display function for graphes
//  ----------------------------------------------------------------------------
ostream& operator<<(ostream& os, Graph graph)
{
    const int slot_width = 5;

    os << setw(slot_width) << " ";
    // Labels
    for (unsigned i = 0; i < graph.nb_vertices_get(); ++i) {
        os << setw(slot_width) << i;
    }
    os << endl;

    // A line
    for (unsigned i = 0; i <= graph.nb_vertices_get(); ++i) {
        os << "-----";
    }
    os << endl;

    // The half matrix with costs.
    for (unsigned i = 0; i < graph.nb_vertices_get(); ++i) {
        os << setw(slot_width - 1) << i << "|";
        for (unsigned j = 0; j < graph.nb_vertices_get(); ++j) {
            //cout << "here " << i << ", " << j << endl;
            if (j <= i) {
                os << setw(slot_width) << " ";
            } else {
                cost_t cost = graph.edge_cost_get(i, j);
                if (cost > 0) {
                    os << setprecision(slot_width - 2) << setw(slot_width)
                       << cost;
                } else {
                    os << setw(slot_width) << " ";
                }
            }
        }
        cout << endl;
    }
    return os;
}

//  ----------------------------------------------------------------------------
/// \brief  Check if an edge exists, regardless of cost.
/// \return True if the edge existed.
//  ----------------------------------------------------------------------------
bool Graph::edge_exists(const int start, const int end) const
{
    auto found_index = find(neighbors[start].begin(), neighbors[start].end(), end);
    return found_index != neighbors[start].end();
}

void Graph::edge_add(const int start, const int end, const cost_t cost)
{
    // Check first that the edge does not exist already, even if it has a
    // different cost.
    if (edge_exists(start, end)) {
        //cerr << __func__ << ": attempting to create an edge that already "
        //     << "exists: " << start << " to " << end << ", cost " << cost
        //     << endl;
        return;
    }
    neighbors[start].push_back(end);
    neighbors[end].push_back(start);
    costs[start][end] = cost;
    costs[end][start] = cost;
    ++nb_edges;
}

//  ----------------------------------------------------------------------------
/// \brief  Set the cost of an edge. The edge must already exist, otherwise use
/// edge_add().
//  ----------------------------------------------------------------------------
void Graph::edge_cost_set(const int start, const int end, const cost_t cost)
{
    if (edge_exists(start, end)) {
        costs[start][end] = cost;
        costs[end][start] = cost;
    } else {
        cerr << __func__ << ": cost_set must have an existing edge." << endl;
    }
}

//  ----------------------------------------------------------------------------
/// \brief  Get the cost of an edge. Returns 0 if the edge was not found.
/// \param  start, end.
/// \return The cost of the edge, 0 if there is no such edge.
//  ----------------------------------------------------------------------------
cost_t Graph::edge_cost_get(const int start, const int end) const
{
    if (edge_exists(start, end)) {
        return costs[start][end];
    }
    return 0;
}

vector<Edge> Graph::all_possible_edges_generate(void) const
{
    vector<Edge> all_edges;
    // No edges for a vertex to itself, thus the -1.
    // The edges are undirected, thus the /2.
    all_edges.reserve(nb_vertices * (nb_vertices - 1) / 2);

    for (int i = 0; i < nb_vertices; ++i) {
        for (int j = i + 1; j < nb_vertices; ++j) {
            Edge edge(i, j, 0);
            all_edges.push_back(edge);
        }
    }
    return all_edges;
}

//  ----------------------------------------------------------------------------
/// \brief  Return the identifier of a node.
//  ----------------------------------------------------------------------------
const int& Graph::node_get(const int index) const
{
    if ((index >= nb_vertices) || (index < 0)) {
        cerr << __func__ << "index out of range!" << endl;
        // What else to do?
    }
    return nodes[index];
}

//  ----------------------------------------------------------------------------
/// \brief  Give a name of identifier to the nodes. So far only a number. Called
/// by all constructors.
//  ----------------------------------------------------------------------------
void Graph::node_names_init()
{
    nodes.resize(nb_vertices);
    for (int i = 0; i < nb_vertices; ++i) {
        nodes[i] = i;
    }
}
