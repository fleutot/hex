/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Östervall
----------------------------------------------------------------------------*/

// Module under test
#include "../prioqueue.h"

#include <cassert>
#include <iostream>

using namespace std;

//******************************************************************************
// Function prototypes
//******************************************************************************
static void test_prioqueue_add();
static void test_prioqueue_remove();
static void test_prioqueue_top();
static void test_prioqueue_pop_top();
static void test_prioqueue_prio_set();


//******************************************************************************
// Function definitions
//******************************************************************************
int main(void)
{
    test_prioqueue_add();
    test_prioqueue_remove();
    test_prioqueue_top();
    test_prioqueue_pop_top();
    test_prioqueue_prio_set();
}


//******************************************************************************
// Internal functions
//******************************************************************************
static void test_prioqueue_add()
{
    cout << __func__ << endl;
    int a = 2;
    int b = 3;
    int c = 4;
    int d = 5;
    int e = 2;

    Prioqueue<int> queue;
    // adds references, need a variable.
    queue.add(a, 1);
    queue.add(b, 4);
    queue.add(c, 3);
    queue.add(d, 2);

    assert(queue.size() == 4);

    queue.add(e, 2);    // Id exists already, replace prio.

    assert(queue.size() == 4);
}

static void test_prioqueue_remove()
{
    cout << __func__ << endl;

    int a = 2;
    int b = 3;
    int c = 4;
    int d = 5;

    Prioqueue<int> queue;
    // adds references, need a variable.
    queue.add(a, 1);
    queue.add(b, 4);
    queue.add(c, 3);
    queue.add(d, 2);

    queue.remove(6);    // Id not in list.
    assert(queue.size() == 4);

    queue.remove(3);    // Id in list.
    assert(queue.size() == 3);
}

static void test_prioqueue_top()
{
    cout << __func__ << endl;

    int a = 2;
    int b = 3;
    int c = 4;
    int d = 5;

    Prioqueue<int> queue;
    // adds references, need a variable.
    queue.add(a, 1);
    queue.add(b, 4);
    queue.add(c, 3);
    queue.add(d, 2);

    int id_in_order[] = {2, 5, 4, 3};

    for (unsigned i = 0; i < 4; ++i) {
        int id = queue.top();
        queue.remove(id);
        assert(id_in_order[i] == id);
    }
    assert(queue.size() == 0);
}

static void test_prioqueue_pop_top()
{
    cout << __func__ << endl;

    int a = 2;
    int b = 3;
    int c = 4;
    int d = 5;

    Prioqueue<int> queue;
    // adds references, need a variable.
    queue.add(a, 1);
    queue.add(b, 4);
    queue.add(c, 3);
    queue.add(d, 2);

    int id_in_order[] = {2, 5, 4, 3};

    for (unsigned i = 0; i < 4; ++i) {
        int id = queue.pop_top();
        assert(id_in_order[i] == id);
    }
    assert(queue.size() == 0);
}

static void test_prioqueue_prio_set()
{
    cout << __func__ << endl;

    int a = 2;
    int b = 3;
    int c = 4;
    int d = 5;

    Prioqueue<int> queue;
    // adds references, need a variable.
    queue.add(a, 1);
    queue.add(b, 4);
    queue.add(c, 3);
    queue.add(d, 2);

    queue.prio_set(5, 1);

    int id_in_order[] = {5, 2, 4, 3};

    for (unsigned i = 0; i < 4; ++i) {
        int id = queue.pop_top();
        assert(id_in_order[i] == id);
    }
}
