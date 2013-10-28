/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Östervall
----------------------------------------------------------------------------*/

#ifndef PRIOQUEUE_H_INCLUDED
#define PRIOQUEUE_H_INCLUDED

#include <iostream>
#include <list>

struct Node {
    int id;
    int prio;
};

class Prioqueue {
public:
    Prioqueue(void);
    ~Prioqueue(void);

    void add(const int id, const int prio);
    void remove(const int id);
    void prio_set(const int id, const int prio);
    bool contains(const int id);
    unsigned size(void);
    int top(void);
    int pop_top(void);
    void print(std::ostream& os);

private:
    std::list<Node> queue;
};

std::ostream& operator<<(std::ostream& os, Prioqueue queue_object);

#endif // PRIOQUEUE_H_INCLUDED
