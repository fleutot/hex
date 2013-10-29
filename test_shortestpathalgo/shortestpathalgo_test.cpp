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
static void test_shortestpathalgo_shortest_pathes_calculate();

//******************************************************************************
// Function definitions
//******************************************************************************
int main(void)
{
    test_shortestpathalgo_constructor();
    test_shortestpathalgo_shortest_pathes_calculate();
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
    cout << "done" << endl;
}

static void test_shortestpathalgo_shortest_pathes_calculate()
{
    cout << __func__ << endl;

    cout << COST_MAX << endl;

    vector<Edge> edges;
    Edge e1(0, 1, 10.0);
    edges.push_back(e1);
    Edge e2(0, 3, 30.0);
    edges.push_back(e2);
    Edge e3(0, 4, 100.0);
    edges.push_back(e3);
    Edge e4(1, 2, 50.0);
    edges.push_back(e4);
    Edge e5(2, 4, 10.0);
    edges.push_back(e5);
    Edge e6(3, 2, 20.0);
    edges.push_back(e6);
    Edge e7(3, 4, 60.0);
    edges.push_back(e7);

    Graph graph(5, edges);
    ShortestPathAlgo algo(graph);

    vector<cost_t> all_distances = algo.shortest_pathes_calculate(0);
    for (vector<cost_t>::iterator it = all_distances.begin(); it != all_distances.end(); ++it) {
        cout << *it << "  ";
    }
}
