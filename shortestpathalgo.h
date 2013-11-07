/*----------------------------------------------------------------------------
ShortestPathAlgo class implementation
----------------------------------------------------------------------------*/

#ifndef SHORTESTPATHALGO_H_INCLUDED
#define SHORTESTPATHALGO_H_INCLUDED

#include <vector>

#include "graph.h"
#include "prioqueue.h"

class ShortestPathAlgo {
public:
    ShortestPathAlgo(const Graph& graph);
    ~ShortestPathAlgo();

    cost_t shortest_path_calculate(const int start, const int end);
    std::vector<cost_t> shortest_pathes_calculate(const int start);
    std::vector<int> shortest_path_from_origin(const int end);
    bool all_targets_done();
    int origin_get();

private:
    const Graph& graph;

    Prioqueue<int> unvisited;
    std::vector<cost_t> distance;
    std::vector<int> previous;  // for each node, the previous node in the
                                // shortest path from origin.
    int current;
    int origin;
    int target;

    bool one_target_done();
    void dijkstra_init();
    void dijkstra_iterate();
};

#endif // SHORTESTPATHALGO_H_INCLUDED
