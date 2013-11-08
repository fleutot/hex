/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Östervall
----------------------------------------------------------------------------*/

// Module under test
#include "../mst_kruskal.h"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

//******************************************************************************
// Function prototypes
//******************************************************************************
static void test_kruskal_constructor(void);
static void test_kruskal_calculate(void);

int main(void)
{
    test_kruskal_constructor();
    test_kruskal_calculate();
    return 0;
}

static void test_kruskal_constructor()
{
    cout << __func__ << endl;
    Graph graph("data.txt");
    volatile MstKruskal algo(graph);
}

static void test_kruskal_calculate()
{
    cout << __func__ << endl;
    Graph graph("data.txt");
    MstKruskal algo(graph);
    cost_t mst_cost = algo.mst_calculate();
    cout << mst_cost << endl;
}
