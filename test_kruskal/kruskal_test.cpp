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

int main(void)
{
    test_kruskal_constructor();
    return 0;
}

static void test_kruskal_constructor()
{
    cout << __func__ << endl;
    Graph graph("data.txt");
    cout << graph;
}
