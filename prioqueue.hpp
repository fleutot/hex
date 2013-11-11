/*----------------------------------------------------------------------------
Prioqueue class implementation.
A prioqueue is a list of Nodes, ordered by prio. The top of the queue is the
node with lowest value in its field "prio".
----------------------------------------------------------------------------*/

#ifndef PRIOQUEUE_H_INCLUDED
#define PRIOQUEUE_H_INCLUDED

#include <iostream>
#include <list>

// Elements in the queue
template <typename T>
struct Node {
    Node(const T& data, const double prio): data(data), prio(prio) {}

    const T& data;
    double prio;
};

template <typename Data>
class Prioqueue {
public:
    Prioqueue(void);
    ~Prioqueue(void);

    void clear(void);

    void add(const Data& data, const double prio);

    void remove(const Data& data);
    void prio_set(const Data& data, const double prio);

    bool contains(const Data& data);
    unsigned size(void);
    Data top(void);
    Data pop_top(void);
    void print(std::ostream& os);

private:
    std::list< Node<Data> > queue;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, Prioqueue<T> queue_object);


// The member functions' implementation needs to be included here, for client
// modules to trigger code generation for the type they require.
#include "prioqueue.t.hpp"


#endif // PRIOQUEUE_H_INCLUDED
