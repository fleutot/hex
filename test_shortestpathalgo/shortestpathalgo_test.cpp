/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Östervall
----------------------------------------------------------------------------*/

// Module under test
#include "../shortestpathalgo.h"
#include "../graph.h"

#include <cassert>
#include <iostream>

using namespace std;

//******************************************************************************
// Function prototypes
//******************************************************************************
static void test_shortestpathalgo_constructor();
static void test_shortestpathalgo_shortest_path_calculate();
static void test_shortestpathalgo_shortest_pathes_calculate();
static void test_shortestpathalgo_shortestpath_from_origin_1();
static void test_shortestpathalgo_shortestpath_from_origin_2();

//******************************************************************************
// Function definitions
//******************************************************************************
int main(void)
{
    test_shortestpathalgo_constructor();
    test_shortestpathalgo_shortest_path_calculate();
    test_shortestpathalgo_shortest_pathes_calculate();
    test_shortestpathalgo_shortestpath_from_origin_1();
    test_shortestpathalgo_shortestpath_from_origin_2();
}


//******************************************************************************
// Internal functions
//******************************************************************************
static void test_shortestpathalgo_constructor()
{
    cout << __func__ << endl;

    const int nb_vertices = 10;
    const double density = 0.5;
    const cost_t max_cost = 10;

    // Random graph.
    Graph graph(nb_vertices, density, max_cost);

    ShortestPathAlgo algo(graph);
}

static void test_shortestpathalgo_shortest_path_calculate()
{
    cout << __func__ << endl;

    Edge e1(0, 1, 10.0);
    Edge e2(0, 3, 30.0);
    Edge e3(0, 4, 100.0);
    Edge e4(1, 2, 50.0);
    Edge e5(2, 4, 10.0);
    Edge e6(3, 2, 20.0);
    Edge e7(3, 4, 60.0);

    vector<Edge> edges;
    edges.push_back(e1);
    edges.push_back(e2);
    edges.push_back(e3);
    edges.push_back(e4);
    edges.push_back(e5);
    edges.push_back(e6);
    edges.push_back(e7);

    Graph graph(5, edges);
    ShortestPathAlgo algo(graph);

    cost_t distance = algo.shortest_path_calculate(0, 2);
    assert(distance == 50);
}

static void test_shortestpathalgo_shortest_pathes_calculate()
{
    cout << __func__ << endl;

    Edge e1(0, 1, 10.0);
    Edge e2(0, 3, 30.0);
    Edge e3(0, 4, 100.0);
    Edge e4(1, 2, 50.0);
    Edge e5(2, 4, 10.0);
    Edge e6(3, 2, 20.0);
    Edge e7(3, 4, 60.0);

    vector<Edge> edges;
    edges.push_back(e1);
    edges.push_back(e2);
    edges.push_back(e3);
    edges.push_back(e4);
    edges.push_back(e5);
    edges.push_back(e6);
    edges.push_back(e7);

    Graph graph(5, edges);
    ShortestPathAlgo algo(graph);

    vector<cost_t> all_distances = algo.shortest_pathes_calculate(0);

    cost_t expected_results[] = {0, 10, 50, 30, 60};

    for (unsigned i = 0; i < all_distances.size(); ++i) {
        assert(all_distances[i] == expected_results[i]);
    }
}

static void test_shortestpathalgo_shortestpath_from_origin_1()
{
    cout << __func__ << endl;

    Edge e1(0, 1, 10.0);
    Edge e2(0, 3, 30.0);
    Edge e3(0, 4, 100.0);
    Edge e4(1, 2, 50.0);
    Edge e5(2, 4, 10.0);
    Edge e6(3, 2, 20.0);
    Edge e7(3, 4, 60.0);

    vector<Edge> edges;
    edges.push_back(e1);
    edges.push_back(e2);
    edges.push_back(e3);
    edges.push_back(e4);
    edges.push_back(e5);
    edges.push_back(e6);
    edges.push_back(e7);

    Graph graph(5, edges);
    ShortestPathAlgo algo(graph);

    vector<cost_t> all_distances = algo.shortest_pathes_calculate(0);
    vector<int> path = algo.shortest_path_from_origin(4);

    int expected_path[] = {0, 3, 2, 4};

    for (unsigned i = 0; i < path.size(); ++i) {
        assert(expected_path[i] == path[i]);
    }
}

static void test_shortestpathalgo_shortestpath_from_origin_2()
{
    cout << __func__ << endl;

    // Test that no path is found if there is none.

    Edge e1(0, 1, 10.0);
    Edge e2(0, 3, 30.0);
    // Edge e3(0, 4, 100.0); no edge to 4
    Edge e4(1, 2, 50.0);
    // Edge e5(2, 4, 10.0); no edge to 4
    Edge e6(3, 2, 20.0);
    // Edge e7(3, 4, 60.0); no edge to 4

    vector<Edge> edges;
    edges.push_back(e1);
    edges.push_back(e2);
    // edges.push_back(e3);
    edges.push_back(e4);
    // edges.push_back(e5);
    edges.push_back(e6);
    // edges.push_back(e7);

    Graph graph(5, edges);
    ShortestPathAlgo algo(graph);

    vector<cost_t> all_distances = algo.shortest_pathes_calculate(0);
    vector<int> path = algo.shortest_path_from_origin(4);

    assert(path.size() == 0);
}
