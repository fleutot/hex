/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Östervall
----------------------------------------------------------------------------*/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <limits>
#include <iostream>
#include <vector>

typedef double cost_t;
extern const cost_t COST_MAX;

class Edge {
public:
    Edge(int start, int end, cost_t cost);
    Edge(int start, int end);
    ~Edge();
    int start_get(void) const;
    int end_get(void) const;
    cost_t cost_get(void) const;
    void cost_set(const cost_t cost);
private:
    int start;
    int end;
    cost_t cost;
};


class Graph {
public:
    Graph(const int nb_vertices);
    Graph(const int nb_vertices, std::vector<Edge> edge_list);
    Graph(const int nb_vertices,
          const double edge_density,
          const cost_t max_cost);
    ~Graph(void);
    void print(void);
    unsigned nb_vertices_get(void) const;
    unsigned nb_edges_get(void) const;
    bool edge_exists(int start, int end, int& index) const;
    bool edge_exists(int start, int end) const;
    void edge_add(int start, int end, const cost_t cost);
    void edge_cost_update(int start, int end, const cost_t cost);
    cost_t edge_cost_get(int start, int end) const;
    void edge_cost_set(int start, int end, const cost_t cost);
    bool adjacent_check(const int node_a, const int node_b);
    std::vector<Edge> all_possible_edges_generate(void);
    std::vector<int> neighbors_get(const int node) const;
private:
    std::vector<Edge> edge_list;
    int nb_vertices;
};

std::ostream& operator<<(std::ostream& os, Graph graph);

#endif // GRAPH_H_INCLUDED
