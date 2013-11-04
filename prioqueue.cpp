/*----------------------------------------------------------------------------
Prioqueue class implementation
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
}

Prioqueue::~Prioqueue(void)
{
}

void Prioqueue::clear(void)
{
    queue.clear();
}

//  ----------------------------------------------------------------------------
/// \brief  Insert a new node at the right position in the prioqueue.
//  ----------------------------------------------------------------------------
void Prioqueue::add(const int id, const double prio)
{
    if (contains(id)) {
        remove(id);
    }

    Node new_node;
    new_node.id = id;
    new_node.prio = prio;

    list<Node>::iterator it = queue.begin();
    while ((it != queue.end()) && (it->prio < prio)) {
        ++it;
    }

    queue.insert(it, new_node);
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

void Prioqueue::prio_set(const int id, const double prio)
{
    if (contains(id)) {
        // add does not create a new queue entry if the id is already
        // present. It deals with placing the new entry at the right position.
        add(id, prio);
    }
}

bool Prioqueue::contains(const int id)
{
    for (list<Node>::iterator it = queue.begin(); it != queue.end(); ++it) {
        if (it->id == id) {
            return true;
        }
    }
    return false;
}

unsigned Prioqueue::size(void)
{
    return queue.size();
}

double Prioqueue::top()
{
    Node first_node = queue.front();
    return first_node.id;
}

double Prioqueue::pop_top()
{
    Node first_node = queue.front();
    queue.pop_front();
    return first_node.id;
}

void Prioqueue::print(ostream& os)
{
    for (list<Node>::iterator it = queue.begin(); it != queue.end(); ++it) {
        os << "(id = " << it->id << ", prio = " << it->prio << ")" << endl;
    }
}
