/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Östervall
----------------------------------------------------------------------------*/

// Module under test
#include "../graph.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

//******************************************************************************
// Function prototypes
//******************************************************************************
void test_graph_constructor_1(void);
void test_graph_constructor_4(void);
void test_graph_edge_add(void);
void test_graph_neighbors_get(void);
void test_graph_edge_exists(void);

int main(void)
{
    srand(time(0)); // Seed only once.

    test_graph_constructor_1();
    test_graph_constructor_4();
    test_graph_edge_add();
    test_graph_neighbors_get();
    test_graph_edge_exists();
    return 0;
}

void test_graph_constructor_1(void)
{
    cout << __func__ << endl;
    Graph graph(10);
    assert(graph.nb_vertices_get() == 10);
}

void test_graph_constructor_4(void)
{
    cout << __func__ << endl;

    Graph graph("data.txt");

    cout << graph;
    assert(graph.nb_vertices_get() == 5);
    assert(graph.nb_edges_get() == 9);
}

void test_graph_edge_add(void)
{
    cout << __func__ << endl;
    Graph graph(5);

    graph.edge_add(1, 2, 2.0);
    graph.edge_add(1, 3, 2.0);
    graph.edge_add(2, 3, 2.0);
    graph.edge_add(1, 3, 3.0); // Edge exists already, not added.

    assert(graph.nb_edges_get() == 3);
}

void test_graph_neighbors_get(void)
{
    cout << __func__ << endl;
    Graph graph(5);

    graph.edge_add(2, 3, 2.0);
    graph.edge_add(2, 4, 2.0);
    graph.edge_add(1, 2, 2.0);

    vector<int> neighbors = graph.neighbors_get(2);

    assert(neighbors.size() == 3);
    assert(neighbors[0] == 3);
    assert(neighbors[1] == 4);
    assert(neighbors[2] == 1);
}

void test_graph_edge_exists(void)
{
    cout << __func__ << endl;

    Graph graph(5);

    graph.edge_add(2, 3, 2.0);
    graph.edge_add(2, 4, 2.0);

    bool found = graph.edge_exists(2, 3);

    assert(found == true);

    found = graph.edge_exists(3, 1);
}
