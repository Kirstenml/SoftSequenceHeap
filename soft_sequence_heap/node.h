//
// Node in a Soft Sequence which stores
// - a key
// - a corruption-set (at the beginning empty)
// - a witness-set (at the beginning empty)
//

#ifndef SOFT_SEQUENCE_HEAP_NODE_H
#define SOFT_SEQUENCE_HEAP_NODE_H

#include "circular_linked_list.h"

template<typename T>
struct node{
public:
    node() {
        this->corruption_set = circular_linked_list<T>();
        this->witness_set = circular_linked_list<T>();
    }

    explicit node(const T& value) {
        this->key = value;
        this->corruption_set = circular_linked_list<T>();
        this->witness_set = circular_linked_list<T>();
    }
    // copy constructor is called when a new object is created from an existing object
    node(const node<T> &other) {
        this->key = other.key;
        // do not copy the circular linked list, move last-pointer because items were not deleted
        this->corruption_set = circular_linked_list<T>(other.corruption_set.last);
        this->witness_set = circular_linked_list<T>(other.witness_set.last);
    }

    // assignment operator (is called when an already initialized object is assigned a new value
    node<T>& operator=(const node<T>& other) {
        if (this == &other) { return *this; }
        node<T> tmp(other);
        std::swap(tmp.corruption_set.last, this->corruption_set.last);
        std::swap(tmp.witness_set.last, this->witness_set.last);
        this->key = tmp.key;
        return *this;
    }

    // node destructor is called in merge and prune and the destructor pass the delete query to circular linked list
    ~node() {}

    T key;
    // circular linked lists should be pointers because in prune we delete a node containing a circular linked list
    // but actually we do not want to kill the cll, just move some pointer
    circular_linked_list<T> corruption_set;
    circular_linked_list<T> witness_set;

};

template<typename T>
std::ostream& operator<<(std::ostream &out, node<T> const& n) {
    out << n.key;
    return out;
}

#endif //SOFT_SEQUENCE_HEAP_NODE_H
