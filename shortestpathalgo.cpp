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
    cost_t shortest_path_distance = COST_MAX;

    cout << "COST_MAX: " << COST_MAX << endl;
    for (unsigned i = 0; i < graph.nb_vertices_get(); ++i) {
        unvisited.add(i, COST_MAX);
        distance[i] = COST_MAX;
        cout << "d" << i << ": " << distance[i] << ", ";
    }
    cout << endl;
    cout << unvisited << endl;
    current = start;
    distance[current] = 0;
    bool done = false;
    cout << "Size of distance: " << distance.size() << endl;

    cout << graph << endl;

    while (!done) {
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

        if (current == end) {
            shortest_path_distance = distance[current];
            done = true;
        }

        if (unvisited.top() == COST_MAX) {
            // Done, no path was found.
            done = true;
        }

        current = unvisited.top();
    }
    if (shortest_path_distance < COST_MAX) {
        cout << "Shortest path has cost " << shortest_path_distance << "."
             << endl;
    } else {
        cout << "No path was found." << endl;
    }
    return shortest_path_distance;
}
