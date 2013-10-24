/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Östervall
----------------------------------------------------------------------------*/

// Module under test
#include "../graph.h"

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

//******************************************************************************
// Function prototypes
//******************************************************************************
void test_graph_constructor_1(void);
void test_graph_constructor_2(void);
void test_graph_edge_add(void);

int main(void)
{
    srand(time(0)); // Seed only once.

    test_graph_constructor_1();
    test_graph_constructor_2();
    test_graph_edge_add();
    return 0;
}

void test_graph_constructor_1(void)
{
    Graph graph(10);
    assert(graph.nb_vertices_get() == 10);
}

void test_graph_constructor_2(void)
{

    vector<Edge> edges;

    Edge e1(1, 2, 2.0);
    edges.push_back(e1);
    Edge e2(1, 2, 2.0);
    edges.push_back(e2);
    Edge e3(1, 2, 2.0);
    edges.push_back(e3);

    Graph graph(5, edges);
    assert(graph.nb_vertices_get() == 5);
    assert(graph.nb_edges_get() == 3);
}

void test_graph_edge_add(void)
{
    Graph graph(5);

    Edge e1(1, 2, 2.0);
    Edge e2(1, 2, 2.0);
    Edge e3(1, 2, 2.0);

    graph.edge_add(e1);
    graph.edge_add(e2);
    graph.edge_add(e3);

    assert(graph.nb_edges_get() == 3);
}
