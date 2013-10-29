/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Östervall
----------------------------------------------------------------------------*/
#include "shortestpathalgo.h"

#include <limits.h>

#include "graph.h"
#include "prioqueue.h"

using namespace std;

ShortestPathAlgo::ShortestPathAlgo(const Graph& graph):
    graph(graph)
{
    // Give room for distances for all nodes.
    distance.resize(graph.nb_vertices_get());
}

ShortestPathAlgo::~ShortestPathAlgo()
{
}

cost_t ShortestPathAlgo::shortest_path_calculate(const int start, const int end)
{
    origin = start;
    target = end;

    dijkstra_init();

    current = start;
    distance[current] = 0;

    bool done = false;
    while (!done) {
        dijkstra_iterate();
        done = all_targets_done() || one_target_done();
    }
    return distance[current];
}

vector<cost_t> ShortestPathAlgo::shortest_pathes_calculate(const int start)
{
    origin = start;
    dijkstra_init();

    current = start;
    distance[current] = 0;

    bool done = false;
    while (!done) {
        dijkstra_iterate();
        done = all_targets_done();
    }

    return distance;
}

void ShortestPathAlgo::dijkstra_init()
{
    for (unsigned i = 0; i < graph.nb_vertices_get(); ++i) {
        unvisited.add(i, COST_MAX);
        distance[i] = COST_MAX;
    }
}

void ShortestPathAlgo::dijkstra_iterate()
{
    cout << "--------------------" << endl;
    cout << "current: " << current << endl;
    vector<int> neighbors = graph.neighbors_get(current);
    cout << "nb neighbors: " << neighbors.size() << endl;
    cout << "unvisited: " << endl << unvisited;

    vector<int>::iterator it;
    for (it = neighbors.begin(); it != neighbors.end(); ++it) {
        cout << "neighbor: " << *it;
        if (unvisited.contains(*it)) {
            cout << ", unvisited";
            // *it is an unvisited neighbor, calculate its tentative distance
            // and set it if it is less than the existing one.
            cost_t tentative_distance = distance[current]
                + graph.edge_cost_get(current, *it);
            cout << ", tentative: " << tentative_distance << ", dist: " << distance[*it];

            if (tentative_distance < distance[*it]) {
                cout << ", new distance for " << *it;
                distance[*it] = tentative_distance;
                unvisited.prio_set(*it, tentative_distance);
            }
        }
        cout << endl;
    }
    // Done updating all neighbors of current, mark as visited.
    unvisited.remove(current);

    current = unvisited.top();
}

bool ShortestPathAlgo::all_targets_done()
{
    return ((unvisited.top() == COST_MAX) || (unvisited.size() == 0));
}

int ShortestPathAlgo::origin_get()
{
    return origin;
}

bool ShortestPathAlgo::one_target_done()
{
    return (current == target);
}
