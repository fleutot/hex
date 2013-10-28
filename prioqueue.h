/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Ostervall
----------------------------------------------------------------------------*/

#ifndef PRIOQUEUE_H_INCLUDED
#define PRIOQUEUE_H_INCLUDED

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
    bool contains(const int id);
    unsigned size(void);

private:
    std::list<Node> queue;
    unsigned queue_size;
};

#endif // PRIOQUEUE_H_INCLUDED
