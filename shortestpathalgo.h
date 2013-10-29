/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Östervall
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
private:
    const Graph& graph;
    Prioqueue unvisited;
    std::vector<cost_t> distance;
    int current;
};

#endif // SHORTESTPATHALGO_H_INCLUDED
