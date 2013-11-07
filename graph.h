/*----------------------------------------------------------------------------
Graph class implementation
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
    Edge(int start = 0, int end = 0, cost_t cost = 0);
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

// These operators only check the vertices that the edges connect, not the cost.
bool operator==(const Edge& a, const Edge& b);
bool operator!=(const Edge& a, const Edge& b);


class Graph {
public:
    Graph(const int nb_vertices);
    Graph(const int nb_vertices, std::vector<Edge> edge_list);
    Graph(const int nb_vertices, const double edge_density,
          const cost_t min_cost, const cost_t max_cost);
    Graph(const std::string filename);
    ~Graph(void);

    void print(void);   // Use rather operator<< to display graphes.

    inline unsigned nb_vertices_get(void) const {
        return nb_vertices;
    }
    inline unsigned nb_edges_get(void) const {
        return edge_list.size();
    }

    cost_t edge_cost_get(int start, int end) const;
    bool adjacent_check(const int node_a, const int node_b) const;

    bool edge_exists(int start, int end, int& index) const;
    bool edge_exists(int start, int end) const;

    const std::vector<Edge>& edge_list_get() const;

    void edge_add(int start, int end, const cost_t cost);
    void edge_cost_set(int start, int end, const cost_t cost);

    std::vector<Edge> all_possible_edges_generate(void) const;
    std::vector<int> neighbors_get(const int node) const;

    const int& node_get(const int index) const;

private:
    void node_names_init();

    std::vector<int> nodes;
    std::vector<Edge> edge_list;
    int nb_vertices;
};

std::ostream& operator<<(std::ostream& os, Graph graph);

#endif // GRAPH_H_INCLUDED
