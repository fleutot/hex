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

//  ----------------------------------------------------------------------------
/// \brief  Graph constructor
/// \param  nb_vertices Number of vertices (nodes) to create.
/// \param  edge_density    Fraction of the total number of possible edges,
/// which must be an actual edge.
/// \param  max_cost    Maximal cost for any of the edges created.
//  ----------------------------------------------------------------------------
Graph::Graph(const int nb_vertices = 50,
             const double edge_density = 0.1,
             const cost_t max_cost = 10)
{
    this->nb_vertices = nb_vertices;

    // nb_possible_edges includes edges from a vertix to itself.
    const int nb_possible_edges = nb_vertices * nb_vertices;

    // edge_matrix: cost at slot (i, j) means there is an edge of that cost
    // between vertix i and j.  All other 0 means no edge there (e.g. on the
    // diagonal of the matrix).  Instead of a 2D array, the matrix is internally
    // represented by a 1D array.  Index [i][j] is instead index [i *
    // nb_vertices + j].  This was done because of the difficulty of dynamically
    // allocating a 2D array.
    edge_matrix = new int[nb_possible_edges]();   // zero-initialized.

    int nb_edges = static_cast<int> (edge_density * nb_possible_edges);
    cout << "nb_edges: " << nb_edges << endl;

    // Create a list of 1D indeces in edge_matrix, which are possible as edges.
    // In a 2D array, these would be any slot that is not on the diagonal.
    // In a 1D array the principle remains: the indeces that may be picked are
    // all (i * width + j) such that i != j.
    vector<int> possible_edges;
    // Excludes edges from a vertix to itself ( - 1).
    possible_edges.reserve(nb_vertices * (nb_vertices - 1));
    for (int i = 0; i < nb_vertices; ++i) {
        for (int j = 0; j < nb_vertices; ++j) {
            if (i != j) {
                possible_edges.push_back(i * nb_vertices + j);
            }
        }
    }

    // Pick a number of random edges amongst the possible ones.
    vector<int> edge_indeces = random_pick(possible_edges, nb_edges);

    // Set a cost to these randombly picked edges.
    for (unsigned int i = 0; i < edge_indeces.size(); ++i) {
        // The "-1" and "+1" to avoid giving cost 0, which means no edge.
        edge_matrix[edge_indeces[i]] = rand() % (max_cost - 1) + 1;
    }
}


//  ----------------------------------------------------------------------------
/// \brief  Destructor
//  ----------------------------------------------------------------------------
Graph::~Graph(void)
{
    delete(edge_matrix);
}


//  ----------------------------------------------------------------------------
cost_t Graph::edge_cost_get(const int start, const int end)
{
    return edge_matrix[start * nb_vertices + end];
}

//  ----------------------------------------------------------------------------
void Graph::edge_cost_set(const int start, const int end, const cost_t cost)
{
    edge_matrix[start * nb_vertices + end] = cost;
}

//  ----------------------------------------------------------------------------
void Graph::print(void)
{
    cout << "    ";
    for (int i = 0; i < nb_vertices; ++i) {
        cout << setw(4) << i;
    }
    cout << endl;

    for (int i = 0; i < nb_vertices; ++i) {
        cout << setw(4) << i;
        for (int j = 0; j < nb_vertices; ++j) {
            cout << setw(4) << edge_cost_get(i, j);
        }
        cout << endl;
    }
}

