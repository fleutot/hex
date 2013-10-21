/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Östervall
----------------------------------------------------------------------------*/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <climits>

typedef int cost_t;
const cost_t COST_MAX = INT_MAX;

class Graph {
public:
    Graph(const int nb_vertices,
          const double edge_density,
          const cost_t max_cost);
    ~Graph(void);
    void print(void);
    cost_t edge_cost_get(const int start, const int end);

private:
    // Dynamically allocated array.
    cost_t *edge_matrix;
    int nb_vertices;
    void edge_cost_set(const int start, const int end, const cost_t cost);
};

#endif // GRAPH_H_INCLUDED
