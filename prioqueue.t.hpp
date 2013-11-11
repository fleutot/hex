/*----------------------------------------------------------------------------
Prioqueue class implementation
This needs to be in a hpp file because it implements a template, and client
modules need to know the functions definitions at compile time.
----------------------------------------------------------------------------*/
#include "prioqueue.hpp"

#include <iostream>
#include <vector>

using namespace std;

template <typename T>
ostream& operator<<(ostream& os, Prioqueue<T> queue_object)
{
    queue_object.print(os);
    return os;
}

//******************************************************************************
// Class implementation
//******************************************************************************
template <typename T>
Prioqueue<T>::Prioqueue(void)
{
}

template <typename T>
Prioqueue<T>::~Prioqueue(void)
{
}

template <typename T>
void Prioqueue<T>::clear(void)
{
    queue.clear();
}

//  ----------------------------------------------------------------------------
/// \brief  Insert a new node at the right position in the prioqueue.
//  ----------------------------------------------------------------------------
template <typename T>
void Prioqueue<T>::add(const T& data, const double prio)
{
    if (contains(data)) {
        remove(data);
    }

    // Lists manage their own memory allocation, so this auto variable is ok.
    Node<T> new_node(data, prio);

    // Linear search, could be improved.
    auto it = queue.begin();
    while ((it != queue.end()) && (it->prio < prio)) {
        ++it;
    }

    queue.insert(it, new_node);
}

template <typename T>
void Prioqueue<T>::remove(const T& data)
{
    auto it = queue.begin();
    while ((it != queue.end()) && (it->data != data)) {
        ++it;
    }
    if (it != queue.end()) {
        // id was found in the list.
        queue.erase(it);
    } else {
        // cerr << __func__ << ": node to remove not found!" << endl;
    }
}

template <typename T>
void Prioqueue<T>::prio_set(const T& data, const double prio)
{
    auto it = queue.begin();
    while ((it != queue.end()) && (it->data != data)) {
        ++it;
    }

    if (it != queue.end()) {
        // Reuse the data field in the list element, not the new one, since data
        // is a reference instead of a value.
        // add does not create a new queue entry if the data is already
        // present. It deals with placing the new entry at the right position.
        add(it->data, prio);
    }
}

template <typename T>
bool Prioqueue<T>::contains(const T& data)
{
    for (auto it = queue.begin(); it != queue.end(); ++it) {
        if (it->data == data) {
            return true;
        }
    }
    return false;
}

template <typename T>
unsigned Prioqueue<T>::size(void)
{
    return queue.size();
}

template <typename T>
T Prioqueue<T>::top()
{
    Node<T> first_node = queue.front();
    return first_node.data;
}

template <typename T>
T Prioqueue<T>::pop_top()
{
    Node<T> first_node = queue.front();
    queue.pop_front();
    return first_node.data;
}

template <typename T>
void Prioqueue<T>::print(ostream& os)
{
    for (auto it = queue.begin(); it != queue.end(); ++it) {
        os << "(data = " << it->data << ", prio = " << it->prio << ")" << endl;
    }
}
