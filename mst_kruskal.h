/*----------------------------------------------------------------------------
MstKruskal class implementation
----------------------------------------------------------------------------*/

#ifndef MST_KRUSKAL_H_INCLUDED
#define MST_KRUSKAL_H_INCLUDED

#include <vector>

#include "graph.h"
#include "prioqueue.h"

class MstKruskal {
public:
    MstKruskal(const Graph& graph): graph(graph) {
        // Init the vertex connection table as nothing connected.
        vertex_connected.resize(graph.nb_vertices_get(), false);
    }
    cost_t mst_calculate(); // returns COST_MAX if the graph was not connected.
    const std::vector<Edge>& mst_edge_list_get();
    cost_t mst_cost_get();  // returns COST_MAX if the graph was not connected.

private:
    cost_t mst_cost = COST_MAX;
    bool mst_done = false;

    std::vector<Edge> mst_edges;
    Prioqueue<Edge> edge_queue;

    std::vector<bool> vertex_connected;

    const Graph& graph;

    void edge_queue_populate();
    void edge_add(const Edge& edge);
    bool mst_done_check();
};

#endif // MST_KRUSKAL_H_INCLUDED
