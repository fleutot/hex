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

const int NO_PREVIOUS = INT_MAX;

ShortestPathAlgo::ShortestPathAlgo(const Graph& graph):
    graph(graph)
{
    // Give room for distances for all nodes.
    distance.resize(graph.nb_vertices_get());
    previous.resize(graph.nb_vertices_get());

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

// ----------------------------------------------------------------------------
/// \brief Shows the shortest path form the start point used in a previous call
/// to shortest path calculation function. Use shortest_pathes_calculate() or
/// shortest_path_calculate() first, then call this function to see the path
/// itself. Note that this function assumes that the target had a path from
/// origin, which was calculated with one of the previous functions. If you used
/// shortest_path_calculate() and the target node was not included in the
/// calculation, it will look like there is no path to end. Therefore the best
/// is to calculate all pathes from origin.
/// \param  end The target node
/// \return A list of node names to reach from origin to end.
/// ----------------------------------------------------------------------------
vector<int> ShortestPathAlgo::shortest_path_from_origin(const int end)
{
    int tracer = end;
    list<int> trace;
    vector<int> whole_trace;    // The return vector.

    if (static_cast<unsigned> (end) >= graph.nb_vertices_get()) {
        return whole_trace; // Error in input parameter end.
    }

    while (tracer != origin)  {
        if (tracer == NO_PREVIOUS) {
            return whole_trace; // Empty vector, no path.
        }
        trace.push_front(tracer);
        tracer = previous[tracer];
    }
    trace.push_front(tracer);   // origin.

    // Copy the list to the return vector.
    whole_trace.reserve(trace.size());
    whole_trace.insert(whole_trace.end(), trace.begin(), trace.end());
    return whole_trace;
}

//  ----------------------------------------------------------------------------
/// \brief  Initialize data structures before running the Dijkstra shortest path
/// algorithm.
//  ----------------------------------------------------------------------------
void ShortestPathAlgo::dijkstra_init()
{
    unvisited.clear();
    for (unsigned i = 0; i < graph.nb_vertices_get(); ++i) {
        unvisited.add(graph.node_get(i), COST_MAX);
        distance[i] = COST_MAX;
        previous[i] = NO_PREVIOUS;
    }
}

//  ----------------------------------------------------------------------------
/// \brief  One loop of the Dijkstra algorithm, as per the Wikipedia page:
/// http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
//  ----------------------------------------------------------------------------
void ShortestPathAlgo::dijkstra_iterate()
{
    vector<int> neighbors = graph.neighbors_get(current);

    for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {
        if (unvisited.contains(*it)) {
            // *it is an unvisited neighbor, calculate its tentative distance
            // and set it if it is less than the existing one.
            cost_t tentative_distance = distance[current]
                + graph.edge_cost_get(current, *it);

            if (tentative_distance < distance[*it]) {
                distance[*it] = tentative_distance;
                previous[*it] = current;
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
