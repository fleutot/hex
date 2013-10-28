/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Östervall
----------------------------------------------------------------------------*/
#include "prioqueue.h"

#include <iostream>
#include <vector>

using namespace std;

ostream& operator<<(ostream& os, Prioqueue queue_object)
{
    queue_object.print(os);
    return os;
}

//******************************************************************************
// Class implementation
//******************************************************************************
Prioqueue::Prioqueue(void)
{
    queue_size = 0;
}

Prioqueue::~Prioqueue(void)
{
}

void Prioqueue::add(const int id, const int prio)
{
    if (contains(id)) {
        remove(id);
    }

    Node new_node = (Node) {
        .id = id,
        .prio = prio
    };

    list<Node>::iterator it = queue.begin();
    while ((it != queue.end()) && (it->prio < prio)) {
        ++it;
    }

    queue.insert(it, new_node);
    queue_size = queue.size();
}

void Prioqueue::remove(const int id)
{
    list<Node>::iterator it = queue.begin();
    while ((it != queue.end()) && (it->id != id)) {
        ++it;
    }
    if (it != queue.end()) {
        // id was found in the list.
        queue.erase(it);
    }
}

bool Prioqueue::contains(const int id)
{
    for (list<Node>::iterator it = queue.begin();
         it != queue.end();
         ++it) {
        if (it->id == id) {
            return true;
        }
    }    return false;
}

unsigned Prioqueue::size(void)
{
    return queue_size;
}

void Prioqueue::print(ostream& os)
{
    for (list<Node>::iterator it = queue.begin(); it != queue.end(); ++it) {
        os << "(id = " << it->id << ", prio = " << it->prio << ")" << endl;
    }
}
