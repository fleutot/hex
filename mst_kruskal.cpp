/*------------------------------------------------------------------------------
MstKruskal class implementation
------------------------------------------------------------------------------*/

#include <vector>

#include "graph.h"
#include "prioqueue.h"

#include "mst_kruskal.h"

using namespace std;


//  ----------------------------------------------------------------------------
/// \brief  Create a queue of all edges in the graph, sorted by cost (low cost
/// is top).
//  ----------------------------------------------------------------------------
void MstKruskal::edge_queue_populate()
{
    const vector<Edge>& all_edges = graph.edge_list_get();
    //for (auto it = all_edges.begin(); it != all_edges.end(); ++it) {
    for (auto edge : all_edges) {
        // Filling the priority queue could be made more efficient.
        edge_queue.add(edge, edge.cost_get());
    }
}

//  ----------------------------------------------------------------------------
/// \brief  Checkt the conditions that end the algorithm. Either a minimum
/// spanning tree is found, or the graph is disconnected.
/// \return True if the algorithm has reached its end.
//  ----------------------------------------------------------------------------
bool MstKruskal::mst_done_check()
{
    return (mst_edges.size() == graph.nb_vertices_get() - 1) // MST found
        || (edge_queue.size() == 0);            // Graph is not connected.
}

//  ----------------------------------------------------------------------------
/// \brief  Add an edge to the solution MST.
/// \param  edge    The edge to add.
//  ----------------------------------------------------------------------------
void MstKruskal::edge_add(const Edge& edge)
{
    mst_edges.push_back(edge);
    vertex_connected[edge.start_get()] = true;
    vertex_connected[edge.end_get()] = true;
}

//  ----------------------------------------------------------------------------
/// \brief  The Kruskal algorithm:
/// - Sort all edges by cost
/// - go through all edges, adding them to the solution if they do not form a
/// loop.
/// \return The cost of the MST if found, COST_MAX otherwise.
//  ----------------------------------------------------------------------------
cost_t MstKruskal::mst_calculate()
{
    edge_queue_populate();

    while (!mst_done_check()) {
        Edge edge = edge_queue.pop_top();
        if (!vertex_connected[edge.start_get()]
            || !vertex_connected[edge.end_get()]
            ) {
            // Add the edge to the solution only if not creating a loop, that is
            // only if not both vertices are already connected.
            edge_add(edge);
        }
    }

    if (edge_queue.size() == 0) {
        // The graph is not connected.
        mst_cost = COST_MAX;
        mst_edges.resize(0);
    }

    mst_done = true;
    return mst_cost;
}

//  ----------------------------------------------------------------------------
/// \brief  Get the list of edges forming the MST. Calculate the MST if not
/// already done.
/// \return The MST.
//  ----------------------------------------------------------------------------
const vector<Edge>& MstKruskal::mst_edge_list_get()
{
    if (!mst_done) {
        // Include the calculation if needed. Discard the return value, it is in
        // the member variable mst_cost anyway.
        mst_calculate();
    }

    return mst_edges;
}

//  ----------------------------------------------------------------------------
/// \brief  Get the cost of the MST, calculate the MST if not already done.
/// \return The cost of the MST.
//  ----------------------------------------------------------------------------
cost_t MstKruskal::mst_cost_get()
{
    if (mst_done) {
        return mst_cost;
    }
    return mst_calculate();
}


