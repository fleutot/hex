/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Östervall
----------------------------------------------------------------------------*/

#include <iostream>
#include <iomanip>  // For width of display
#include <cstdlib>
#include <vector>

#include "graph.h"

using namespace std;

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
/// \brief  Calculate a random cost between 1.0 and max_cost.
/// Note: this would not work if cost_t was integer.
/// \param  Max cost
/// \return Random cost.
//  ----------------------------------------------------------------------------
static cost_t random_cost_calculate(const cost_t max_cost)
{
    return (static_cast<cost_t> (rand()) / RAND_MAX) * (max_cost - 1) + 1;
}

Edge::Edge(int start, int end, cost_t cost)
{
    if (start == end) {
        cout << "Created an edge with same start and end! (" << start
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

//  ----------------------------------------------------------------------------
/// \brief  Create an edge with no cost, use to create temp edges, for example
/// as input parameter to edge_exists.
/// \param  start, end: vertices indexes.
//  ----------------------------------------------------------------------------
Edge::Edge(int start, int end)
{
    if (start == end) {
        cout << "Created an edge with same start and end! (" << start
             << ")" << endl;
        exit(1);
    } else if (start > end) {
        // Edges are undirected, have always start < end as a convention.
        // This may speed up searches.
        this->start = end;
        this->end = start;
    } else {
        this->start = start;
        this->end = end;
    }
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

//  ----------------------------------------------------------------------------
/// \brief  Graph constructor
/// \param  nb_vertices Number of vertices (nodes) to create.
/// \param  edge_density    Fraction of the total number of possible edges,
/// which must be an actual edge.
/// \param  max_cost    Maximal cost for any of the edges created.
//  ----------------------------------------------------------------------------
Graph::Graph(const int nb_vertices = 50)
{
    this->nb_vertices = nb_vertices;
}

Graph::Graph(const int nb_vertices, vector<Edge> edge_list)
{
    this->nb_vertices = nb_vertices;
    this->edge_list = edge_list;
}

// Constructor for a random graph.
Graph::Graph(const int nb_vertices,
             const double edge_density,
             const cost_t max_cost)
{
    this->nb_vertices = nb_vertices;

    // -1 because there are no edges from a vertex to itself.
    // /2 because edges are undirectional.
    int nb_edges = static_cast<int> (nb_vertices * (nb_vertices - 1)
                                     * edge_density / 2);

    vector<Edge> possible_edges = all_possible_edges_generate();
    edge_list = random_pick(possible_edges, nb_edges);

    for (unsigned i = 0; i < edge_list.size(); ++i) {
        edge_list[i].cost_set(random_cost_calculate(max_cost));
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
    for (int i = 0; i < graph.nb_vertices_get(); ++i) {
        os << setw(slot_width) << i;
    }
    os << endl;

    // A line
    for (int i = 0; i <= graph.nb_vertices_get(); ++i) {
        os << "-----";
    }
    os << endl;

    // The half matrix with costs.
    for (int i = 0; i < graph.nb_vertices_get(); ++i) {
        os << setw(slot_width - 1) << i << "|";
        for (int j = 0; j < graph.nb_vertices_get(); ++j) {
            //cout << "here " << i << ", " << j << endl;
            if (j <= i) {
                os << setw(slot_width) << " ";
            } else {
                Edge edge(i, j);
                graph.edge_cost_update(edge);
                cost_t cost = edge.cost_get();
                if (cost > 0) {
                    os << setw(slot_width) << edge.cost_get();
                } else {
                    os << setw(slot_width) << " ";
                }
            }
        }
        cout << endl;
    }
    return os;
}


int Graph::nb_vertices_get(void)
{
    return nb_vertices;
}

int Graph::nb_edges_get(void)
{
    return edge_list.size();
}

//  ----------------------------------------------------------------------------
/// \brief  Check if an edge exists, regardless of cost.
/// \return True if the edge existed.
/// \return index: the index in the edge list if the edge existed.
//  ----------------------------------------------------------------------------
bool Graph::edge_exists(const Edge edge, int& index)
{
    for (unsigned int i = 0; i < edge_list.size(); ++i) {
        if ((edge.start_get() == edge_list[i].start_get())
            && (edge.end_get() == edge_list[i].end_get())
            ) {
            index = i;
            return true;
        }
    }
    return false;
}

//  ----------------------------------------------------------------------------
/// \brief  Check if an edge exists, regardless of cost.
/// \return True if the edge existed.
//  ----------------------------------------------------------------------------
bool Graph::edge_exists(const Edge edge)
{
    for (unsigned int i = 0; i < edge_list.size(); ++i) {
        if ((edge.start_get() == edge_list[i].start_get())
            && (edge.end_get() == edge_list[i].end_get())
            ) {
            return true;
        }
    }
    return false;
}

void Graph::edge_add(const Edge new_edge)
{
    // Check first that the edge does not exist already, even if it has a
    // different cost.
    int edge_found_index = ~0;
    if (edge_exists(new_edge, edge_found_index)) {
        cout << __func__ << ": attempting to create an edge that already "
             << "exists." << endl;
        return;
    }

    edge_list.push_back(new_edge);
}

//  ----------------------------------------------------------------------------
/// \brief  Update the cost of the edge passed as parameter, to the cost of the
/// corresponding edge in the graph if it exists, or to 0 otherwise.
/// \param  edge to update.
//  ----------------------------------------------------------------------------
void Graph::edge_cost_update(Edge& edge)
{
    int found_index = 0;
    bool found = edge_exists(edge, found_index);
    if (found) {
        edge.cost_set(edge_list[found_index].cost_get());
    } else {
        edge.cost_set(0);
    }
}


bool Graph::adjacent_check(int node_a, int node_b)
{
    // Make sure node_a has the lower value of the two, as it is the standard
    // for Edge to have start less than end.
    reorder_values(node_a, node_b);
    for (unsigned int i = 0; i < edge_list.size(); ++i) {
        if ((edge_list[i].start_get() == node_a)
             && (edge_list[i].end_get() == node_b)
            ){
            return true;
        }
    }
    return false;
}

vector<Edge> Graph::all_possible_edges_generate(void)
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
/// \brief  Lists all nodes y such that there is an edge from node to y.
//  ----------------------------------------------------------------------------
vector<int> Graph::neighbors_get(const int node)
{
    vector<int> neighbors;
    for (unsigned int i = 0; i < edge_list.size(); ++i) {
        if ((edge_list[i].start_get() == node)
            || (edge_list[i].end_get() == node)
            ){
            neighbors.push_back(edge_list[i].end_get());
        }
    }
    return neighbors;
}
