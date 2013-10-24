/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Östervall
----------------------------------------------------------------------------*/

// Module under test
#include "../graph.h"

#include <cassert>
#include <cstdlib>
#include <ctime>


//******************************************************************************
// Function prototypes
//******************************************************************************
void test_graph_constructor(void);

int main(void)
{
    srand(time(0)); // Seed only once.

    test_graph_constructor();
    return 0;
}

void test_graph_constructor(void)
{
    const int nb_vertices = 5;
    const double edge_density = 0.5;

    Graph graph(nb_vertices, edge_density, 10);
    graph.print();

    int nb_edges_calculated = 0;
    for (int start = 0; start < nb_vertices; ++start) {
        for (int end = 0; end < nb_vertices; ++end) {
            if (graph.edge_cost_get(start, end) != 0) {
                ++nb_edges_calculated;
            }
        }
    }

    assert(nb_edges_calculated ==
           static_cast<int> (nb_vertices * nb_vertices * edge_density)
        );
}
