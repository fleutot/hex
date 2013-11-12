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
void test_graph_constructor_2(void);
void test_graph_constructor_3(void);
void test_graph_constructor_4(void);
void test_graph_edge_add(void);
void test_graph_neighbors_get(void);
void test_graph_edge_exists(void);
void test_graph_all_possible_edges_generate(void);
void test_graph_type(void);

int main(void)
{
    srand(time(0)); // Seed only once.

    test_graph_constructor_1();
    test_graph_constructor_2();
    test_graph_constructor_3();
    test_graph_constructor_4();
    test_graph_edge_add();
    test_graph_neighbors_get();
    test_graph_edge_exists();
    test_graph_all_possible_edges_generate();
    test_graph_type();
    return 0;
}

void test_graph_constructor_1(void)
{
    cout << __func__ << endl;
    Graph graph(10);
    assert(graph.nb_vertices_get() == 10);
}

void test_graph_constructor_2(void)
{
    cout << __func__ << endl;
    vector<Edge> edges;

    Edge e1(1, 2, 2.0);
    edges.push_back(e1);
    Edge e2(1, 3, 2.0);
    edges.push_back(e2);
    Edge e3(2, 3, 2.0);
    edges.push_back(e3);

    Graph graph(5, edges);
    assert(graph.nb_vertices_get() == 5);
    assert(graph.nb_edges_get() == 3);
}

void test_graph_constructor_3(void)
{
    cout << __func__ << endl;

    const int nb_vertices = 8;
    const double edge_density = 0.5;

    Graph graph(nb_vertices, 0.5, 1.0, 10.0);

    assert(graph.nb_edges_get()
           == static_cast<int> (nb_vertices
                                * (nb_vertices - 1)
                                * edge_density / 2));
    cout << graph;
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

    int found_index = ~0;
    bool found = graph.edge_exists(2, 3, found_index);

    assert(found == true);
    assert(found_index != ~0);

    found_index = ~0;
    found = graph.edge_exists(3, 1, found_index);

    assert(found == false);
    assert(found_index == ~0);
}

void test_graph_all_possible_edges_generate(void)
{
    cout << __func__ << endl;

    const int nb_vertices = 3;

    Graph graph(nb_vertices);
    vector<Edge> all_gen = graph.all_possible_edges_generate();
    Graph full_graph(graph.nb_vertices_get(), all_gen);

    assert(all_gen.size() == (nb_vertices * (nb_vertices - 1)) / 2);

    for (unsigned i = 0; i < full_graph.nb_vertices_get(); ++i) {
        for (unsigned j = 0; j < full_graph.nb_vertices_get(); ++j) {
            if (i != j) {
                assert(full_graph.edge_exists(i, j));
            }
        }
    }
}

void test_graph_type(void)
{
    cout << __func__ << endl;
    Graph graph("data.txt");
    graph.edge_type_set(0, 2, 10);
    assert(graph.edge_type_get(2, 0) == 10);
    assert(graph.edge_type_get(1, 0) == 0);
}
