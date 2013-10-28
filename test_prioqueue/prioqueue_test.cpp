/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Östervall
----------------------------------------------------------------------------*/

// Module under test
#include "../prioqueue.h"

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

//******************************************************************************
// Function prototypes
//******************************************************************************
static void test_prioqueue_add(void);

int main(void)
{
    test_prioqueue_add();
}

//******************************************************************************
// Internal functions
//******************************************************************************
static void test_prioqueue_add(void)
{
    cout << __func__ << endl;
    Prioqueue queue;
    queue.add(2, 1);
    queue.add(3, 4);
    queue.add(4, 3);
    queue.add(5, 2);

    assert(queue.size() == 4);

    queue.add(2, 2);    // Id exists already, replace prio.

    assert(queue.size() == 4);
}
