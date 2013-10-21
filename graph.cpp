/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Östervall
----------------------------------------------------------------------------*/

#include <iostream>
#include <iomanip>  // For width of display
#include <cstdlib>

#include "graph.h"

using namespace std;

Graph::Graph(const int nb_vertices = 50,
             const double edge_density = 0.1,
             const cost_t max_cost = 10)
{
    int nb_possible_edges = nb_vertices * nb_vertices;
    edge_matrix = new int[nb_possible_edges]();   // zero-initialized.
    this->nb_vertices = nb_vertices;

    int nb_edges = static_cast<int> (edge_density * nb_possible_edges);
    cout << "nb_edges: " << nb_edges << endl;
    for (int i = 0; i < nb_edges; ++i) {
        int start, end;
        do {
            start = rand() % nb_vertices;
            do {
                end = rand() % nb_vertices;
            } while (end == start); // no edge from a vertix to itself.
        } while (edge_cost_get(start, end) != 0); // no edge that already exists.
        edge_cost_set(start, end, rand() % max_cost);
    }
}


Graph::~Graph(void)
{
    delete(edge_matrix);
}


cost_t Graph::edge_cost_get(const int start, const int end)
{
    //cout << __func__ << " (" << start << ", " << end << ") -> " << start * nb_vertices + end << endl;
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

