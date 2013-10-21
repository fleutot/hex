/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Östervall
----------------------------------------------------------------------------*/

// Module under test
#include "../graph.h"

#include <cstdlib>
#include <ctime>


//******************************************************************************
// Function prototypes
//******************************************************************************
void test_graph_constructor(void);

int main(void)
{
    srand(time(0)); // Done only once.

    test_graph_constructor();
    return 0;
}

void test_graph_constructor(void)
{
    #warning Test with density one.
    Graph graph(5, 0.5, 10);
    graph.print();
}
