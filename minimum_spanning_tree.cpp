/*------------------------------------------------------------------------------
Run Kruskal's minimum spanning tree algorithm on the data in the file passed as
parameter.
------------------------------------------------------------------------------*/

#include <iostream>
#include <vector>

#include "graph.h"
#include "mst_kruskal.h"

using namespace std;

int main (int argc, char *argv[])
{
    if (argc != 2) {
        cout << "Usage: mst <filename>" << endl;
        exit(1);
    }

    // Assuming argument 1 has the correct format.
    Graph graph(argv[1]);
    MstKruskal algo(graph);

    cost_t mst_cost = algo.mst_cost_get();
    vector<Edge> mst = algo.mst_edge_list_get();

    cout << "Cost of the minimum spanning tree: " << mst_cost << endl;
    cout << "Edges of the mst:" << endl;
    for (auto edge: mst) {
        cout << edge << endl;
    }

    return 0;
}
