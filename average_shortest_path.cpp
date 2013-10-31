/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Östervall
----------------------------------------------------------------------------*/
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "graph.h"
#include "shortestpathalgo.h"

using namespace std;

const int nb_test_graphes = 1000;

const int nb_vertices = 50;
const cost_t max_cost = 10.0;


// ----------------------------------------------------------------------------
/// \brief Average of the content of the input, but consider only the elements
/// within the limits passed as second and third parameters.  This is useful if
/// elements worth 0 or max are to be disregarded.
/// ----------------------------------------------------------------------------
template <class T>
inline double average(const vector<T> input, const T too_low_limit,
                      const T too_high_limit)
{
    T sum = 0;
    unsigned divider = 0;

    for (unsigned i = 0; i < input.size(); ++i) {
        if ((input[i] > too_low_limit) && (input [i] < too_high_limit)) {
            sum += input[i];
            ++divider;
        }
    }
    return static_cast<double> (sum) / divider;
}

//  ----------------------------------------------------------------------------
/// \brief  Create a random graph of the density passed as parameter, calculate
/// all shortest paths from one node to all other, and their average.
/// \param  density Density of the graph to create.
/// \return The average shortest path for the graph created.
//  ----------------------------------------------------------------------------
double average_shortest_path_one_start(const double density)
{
    Graph graph(nb_vertices, density, max_cost);
    ShortestPathAlgo algo(graph);

    // Compute all distances from start point 0.
    vector<cost_t> distances = algo.shortest_pathes_calculate(0);
    return average(distances, 0.0, COST_MAX);
}

//  ----------------------------------------------------------------------------
/// \brief  Monte-carlo simulation, calculate the average shortest path for
/// randomly generated graphs of density 20 % and 40 %. Do that many times
/// (nb_test_graphes) and calculate the average of these averages.  Single runs
/// of the average shortest path showed rather large variations.
//  ----------------------------------------------------------------------------
int main(void)
{
    // Requirement for random graph generation. Must be called only once.
    srand(time(0));

    vector<double> shortest_values_20;
    vector<double> shortest_values_40;
    shortest_values_20.reserve(nb_test_graphes);
    shortest_values_40.reserve(nb_test_graphes);

    for (int i = 0; i < nb_test_graphes; ++i) {
        shortest_values_20.push_back(average_shortest_path_one_start(0.2));
        shortest_values_40.push_back(average_shortest_path_one_start(0.4));
    }
    cout << "Average distance, 20 %: "
         << average(shortest_values_20, 0.0, COST_MAX) << endl;
    cout << "Average distance, 40 %: "
         << average(shortest_values_40, 0.0, COST_MAX) << endl;
}
