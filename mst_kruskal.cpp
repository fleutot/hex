/*------------------------------------------------------------------------------
MstKruskal class implementation
As per the wikipedia article:
http://en.wikipedia.org/wiki/Kruskal%27s_algorithm
------------------------------------------------------------------------------*/

#include <vector>

#include "graph.hpp"
#include "prioqueue.hpp"

#include "mst_kruskal.hpp"

using namespace std;

//  ----------------------------------------------------------------------------
/// \brief  Constructor. Initialize data internal to the algorithm.
/// \param  Graph to run the Kruskal algorithm on.
//  ----------------------------------------------------------------------------
MstKruskal::MstKruskal(const Graph& graph): mst_cost(0), graph(graph)
{
    // Init the vertex connection table as nothing connected.
    vertex_connected.resize(graph.nb_vertices_get(), false);
    // Init all trees, one vertex per tree.
    trees.resize(graph.nb_vertices_get());
    for (unsigned i = 0; i < trees.size(); ++i) {
        trees[i].push_back(i);
    }
}

//  ----------------------------------------------------------------------------
/// \brief  Create a queue of all edges in the graph, sorted by cost (low cost
/// is top).
//  ----------------------------------------------------------------------------
void MstKruskal::edge_queue_populate()
{
    const vector<Edge>& all_edges = graph.edge_list_get();
    for (auto it = all_edges.begin(); it != all_edges.end(); ++it) {
        // Filling the priority queue could be made more efficient.
        edge_queue.add(*it, it->cost_get());
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
    mst_cost += edge.cost_get();
}

// -----------------------------------------------------------------------------
/// \brief  Concatenate two trees into the first one, and discard the second.
// -----------------------------------------------------------------------------
void MstKruskal::trees_merge(unsigned index_a, unsigned index_b)
{
    trees[index_a].reserve(trees[index_a].size() + trees[index_b].size());
    trees[index_a].insert(trees[index_a].end(),
                          trees[index_b].begin(), trees[index_b].end());
    trees.erase(trees.begin() + index_b);
}

int MstKruskal::containing_tree_get(int vertex)
{
    for (unsigned i = 0; i < trees.size(); ++i) {
        for (auto it = trees[i].begin(); it != trees[i].end(); ++it) {
            if (*it == vertex) {
                return i;
            }
        }
    }
    cerr << "Vertex " << vertex << "not found in any tree." << endl;
    return 0;
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

        int tree_index_start = containing_tree_get(edge.start_get());
        int tree_index_end = containing_tree_get(edge.end_get());

        if (tree_index_start != tree_index_end) {
            trees_merge(tree_index_start, tree_index_end);
            // Add the edge to the solution only if not creating a loop, that is
            // only if not both vertices are already in the same tree.
            edge_add(edge);
        }
    }

    if (trees.size() > 1) {
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


