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
struct Node {
    int id;
    double prio;
};

class Prioqueue {
public:
    Prioqueue(void);
    ~Prioqueue(void);

    void clear(void);

    void add(const int id, const double prio);
    void remove(const int id);
    void prio_set(const int id, const double prio);
    bool contains(const int id);
    unsigned size(void);
    double top(void);
    double pop_top(void);
    void print(std::ostream& os);

private:
    std::list<Node> queue;
};

std::ostream& operator<<(std::ostream& os, Prioqueue queue_object);

#endif // PRIOQUEUE_H_INCLUDED
