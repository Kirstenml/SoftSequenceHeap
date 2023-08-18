//
// Circular linked list to save Corruption-sets and Witness-sets
//

#ifndef SOFT_SEQUENCE_HEAP_CIRCULAR_LINKED_LIST_H
#define SOFT_SEQUENCE_HEAP_CIRCULAR_LINKED_LIST_H

#include <iostream>
#include <list>

template<typename T>
struct item{
    item(const T &v) : value(v), next(nullptr) {}
    item() : next(nullptr) {}
    T value;
    item<T> *next;

    ~item() {
        next = nullptr;
    }
};

template<typename T>
class circular_linked_list{
private:
    void insert_item(const T& value);
public:
    item<T> *last;

    circular_linked_list() : last(nullptr) {};
    circular_linked_list(item<T> *i) : last(i) {};
    void append(const T& value);
    void append(item<T> *cll);
    T front();
    T pop_front();
    T delete_next_item(item<T> *it);
    bool empty();
    void delete_cll();
    void to_list(std::list<T> &l);
    size_t size();

    // copy constructor
    circular_linked_list(const circular_linked_list<T> &other) = delete;

    // destructor is only needed when destroying a soft sequence heap
    ~circular_linked_list() = default;
};

#include "circular_linked_list.tpp"


#endif //SOFT_SEQUENCE_HEAP_CIRCULAR_LINKED_LIST_H
