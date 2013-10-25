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
    vector<T> out(nb_out);

    for (int i = 0; i < nb_out; ++i) {
        int index = rand() % input.size();
        // Put the newly picked element in the output vector.
        out[i] = input[index];
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
void reorder_values(T& a, T& b)
{
    if (b < a) {
        T temp = b;
        b = a;
        a = temp;
    }
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

//  ----------------------------------------------------------------------------
/// \brief  Destructor
//  ----------------------------------------------------------------------------
Graph::~Graph(void)
{
    // Nothing to deallocate.
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

void Graph::edge_add(const Edge new_edge)
{
    // Check first that the edge does not exist already, even if it has a
    // different cost.


    edge_list.push_back(new_edge);
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
