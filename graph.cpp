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
        out.push_back(input[index]);
        input.erase(input.begin() + index);
    }
    return out;
}

Graph::Graph(const int nb_vertices = 50,
             const double edge_density = 0.1,
             const cost_t max_cost = 10)
{
    // Includes edges from a vertix to itself.
    const int nb_possible_edges = nb_vertices * nb_vertices;
    edge_matrix = new int[nb_possible_edges]();   // zero-initialized.
    this->nb_vertices = nb_vertices;

    int nb_edges = static_cast<int> (edge_density * nb_possible_edges);
    cout << "nb_edges: " << nb_edges << endl;

    // Create a list of 1D indeces in edge_matrix, which are possible as edges.
    // In a 2D array, these would be any slot that is not on the diagonal.
    // This is implemented as a 1D array but the principle remains: the indeces
    // that may be picked are all (i * width + j) such that i != j.
    vector<int> possible_edges;
    // Excludes edges from a vertix to itself.
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
        edge_matrix[edge_indeces[i]] = rand() % max_cost;
    }
}


Graph::~Graph(void)
{
    delete(edge_matrix);
}


cost_t Graph::edge_cost_get(const int start, const int end)
{
    return edge_matrix[start * nb_vertices + end];
}


void Graph::edge_cost_set(const int start, const int end, const cost_t cost)
{
    edge_matrix[start * nb_vertices + end] = cost;
}

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

