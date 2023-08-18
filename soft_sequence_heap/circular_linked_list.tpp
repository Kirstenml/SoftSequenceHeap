//
// Circular linked list to save Corruption-sets and Witness-sets
//
#ifndef SOFTSEQUENCEHEAP_CIRCULAR_LINKED_LIST_TPP
#define SOFTSEQUENCEHEAP_CIRCULAR_LINKED_LIST_TPP


#include "circular_linked_list.h"


// receives last item in circular linked list and copy very item and return new last item
template<typename T>
item<T> *copy_items(item<T> *other) {
    item<T> *item_n = new item<T>(other->value);
    item<T> *current_next = other->next;
    item<T> *current = item_n;
    item<T> *end = other;
    while (current_next != end) {
        current->next = new item<T>(current_next->value);
        current_next = current_next->next;
        current = current->next;
    }
    current->next = item_n;
    return item_n;
}

template<typename T>
void circular_linked_list<T>::insert_item(const T& value) {
    // insert_head_at_front in empty list
    if (this->last == nullptr){
        auto *temp = new item<T>(value);
        this->last = temp;
        this->last->next = this->last;
    }
}

template<typename T>
void circular_linked_list<T>::append(const T& value) {
    if (this->last == nullptr) {    // insert first item in cll
        insert_item(value);
        return;
    }
    auto *new_item = new item<T>(value);
    new_item->next = this->last->next;
    this->last->next = new_item;
    this->last = new_item;
}

template<typename T>
void circular_linked_list<T>::append(item<T> *other_last) {
    if (this->last == nullptr) {
        this->last = other_last;
        return;
    }
    item<T> *other_first = other_last->next;
    other_last->next = this->last->next;
    this->last->next = other_first;
    this->last = other_first;
}

template<typename T>
T circular_linked_list<T>::front() {
    return this->last->next->value;
}

template<typename T>
T circular_linked_list<T>::pop_front() {
    if (this->last == nullptr) {
        throw "Error during deletion: Circular Linked List is empty!";
    }
    T val = this->last->next->value;
    if (this->last == this->last->next) {   // circular linked list contains only one element
        delete this->last;
        this->last = nullptr;
        return val;
    }
    // circular linked list has at least two items
    item<T> *tmp = this->last->next->next;  // second item in the circular linked list
    delete this->last->next;
    this->last->next = tmp;
    tmp = nullptr;
    return val;
}

// deletes the next item of it and returns the value of the deleted item
template<typename T>
T circular_linked_list<T>::delete_next_item(item<T> *it) {
    if (this->last == nullptr) {
        throw "Error during deletion: Circular Linked List is empty";
    }
    T val = it->next->value;
    if (this->last == this->last->next) {   // circular linked list contains only one element
        delete this->last;
        this->last = nullptr;
    } else {
        if (this->last == it->next) { this->last = it; }
        item<T> *tmp = it->next->next;
        delete it->next;
        it->next = tmp;
    }
    return val;
}

template<typename T>
bool circular_linked_list<T>::empty() {
    return (this->last == nullptr);
}

// we only delete a circular linked list if we delete the whole soft sequence heap (see destructor of ssh)
// items could also be deleted during extract-min
template<typename T>
void circular_linked_list<T>::delete_cll() {
    if (last) { // cll is not empty
        item<T> *tmp = last->next;
        item<T> *tmp_next;
        item<T> *end = last;
        while (tmp != end) {
            tmp_next = tmp->next;
            delete tmp;
            tmp = tmp_next;
        }
        delete tmp; // delete the last item
    }
}

template<typename T>
void circular_linked_list<T>::to_list(std::list<T> &l) {
    if (last == nullptr) { return; }
    item<T> *tmp = last->next;
    item<T> *tmp_next;
    item<T> *end = last;
    while (tmp != end) {
        tmp_next = tmp->next;
        l.push_back(tmp->value);
        delete tmp;
        tmp = tmp_next;
    }
    l.push_back(tmp->value);
    delete tmp;
}

template<typename T>
size_t circular_linked_list<T>::size() {
    size_t cll_size = 0;
    if (last == nullptr) { return cll_size; }
    ++cll_size;
    item<T> *tmp = last->next;
    item<T> *tmp_next;
    item<T> *end = last;
    while (tmp != end) {
        ++cll_size;
        tmp = tmp->next;
    }
    return cll_size;
}

template<typename T>
std::ostream& operator<<(std::ostream &out, circular_linked_list<T> const& cll) {
    out << "{";
    item<T> *current = cll.last;
    if (current == nullptr) {
        out << "}";
        return out;
    }
    current = current->next;
    out << current->value;
    if (current != cll.last) { out << ", "; }
    while (current != cll.last) {
        current = current->next;
        out << current->value;
        if (current != cll.last) { out << ", "; }
    }
    out << "}";
    return out;
}

#endif //SOFTSEQUENCEHEAP_CIRCULAR_LINKED_LIST_TPP
