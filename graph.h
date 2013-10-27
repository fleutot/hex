/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Östervall
----------------------------------------------------------------------------*/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <climits>
#include <iostream>
#include <vector>

typedef double cost_t;
const cost_t COST_MAX = INT_MAX;


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
    int nb_vertices_get(void);
    int nb_edges_get(void);
    bool edge_exists(const Edge edge, int& index);
    bool edge_exists(const Edge edge);
    void edge_add(Edge new_edge);
    void edge_cost_update(Edge& edge);
    bool adjacent_check(const int node_a, const int node_b);
    std::vector<Edge> all_possible_edges_generate(void);
    std::vector<int> neighbors_get(const int node);
private:
    std::vector<Edge> edge_list;
    int nb_vertices;
};

std::ostream& operator<<(std::ostream& os, Graph graph);

#endif // GRAPH_H_INCLUDED