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

//------------------------------------------------------------------------------
class Edge {
public:
    Edge(int start, int end, cost_t cost = 1);
    ~Edge();
    int start_get(void) const;
    int end_get(void) const;

    cost_t cost_get(void) const;
    void cost_set(const cost_t cost);

    friend std::ostream& operator<<(std::ostream& os, const Edge& e);

private:
    int start;
    int end;
    cost_t cost;
};

// These operators only check the vertices that the edges connect, not the cost.
bool operator==(const Edge& a, const Edge& b);
bool operator!=(const Edge& a, const Edge& b);
std::ostream& operator<<(std::ostream& os, const Edge& e);


//------------------------------------------------------------------------------
class Graph {
public:
    Graph(const int nb_vertices);
    Graph(const std::string filename);
    ~Graph(void);

    void print(void);   // Use rather operator<< to display graphes.

    unsigned nb_vertices_get(void) const { return nb_vertices; }
    unsigned nb_edges_get(void) const { return nb_edges; }

    bool edge_exists(const int start, const int end) const;
    void edge_add(const int start, const int end, const cost_t cost = 1);
    void edge_cost_set(const int start, const int end, const cost_t cost);
    cost_t edge_cost_get(const int start, const int end) const;

    std::vector<Edge> all_possible_edges_generate(void) const;
    std::vector<int> neighbors_get(const int node) const {
        return neighbors[node];
    }

    const int& node_get(const int index) const;

private:
    void node_names_init();

    std::vector<int> nodes;
    std::vector< std::vector<int> > neighbors;
    std::vector< std::vector<cost_t> > costs;

    int nb_vertices;
    int nb_edges;
};

std::ostream& operator<<(std::ostream& os, Graph graph);

#endif // GRAPH_H_INCLUDED
