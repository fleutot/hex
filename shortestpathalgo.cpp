/*----------------------------------------------------------------------------
ShortestPathAlgo class implementation
Dijkstra's shortest path algorithm, as per the Wikipedia page:
/// http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
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

//  ----------------------------------------------------------------------------
/// \brief  One loop of the Dijkstra algorithm, as per the Wikipedia page:
/// http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
//  ----------------------------------------------------------------------------
void ShortestPathAlgo::dijkstra_iterate()
{
    vector<int> neighbors = graph.neighbors_get(current);

    vector<int>::iterator it;
    for (it = neighbors.begin(); it != neighbors.end(); ++it) {
        if (unvisited.contains(*it)) {
            // *it is an unvisited neighbor, calculate its tentative distance
            // and set it if it is less than the existing one.
            cost_t tentative_distance = distance[current]
                + graph.edge_cost_get(current, *it);

            if (tentative_distance < distance[*it]) {
                distance[*it] = tentative_distance;
                unvisited.prio_set(*it, tentative_distance);
            }
        }
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
