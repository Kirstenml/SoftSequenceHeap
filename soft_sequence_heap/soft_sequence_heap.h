//
// Soft Sequence Heaps (an alternative implementation of Chazelle's soft heaps) introduced 2021 by Brodal
//

#ifndef SOFT_SEQUENCE_HEAP_SOFT_HEAP_H
#define SOFT_SEQUENCE_HEAP_SOFT_HEAP_H

#include "node.h"
#include <cmath>
#include <cstdint>
#include <list>
#include <cmath>
#include <iostream>
#include <iterator>
#include "head.h"
#include "merge.h"
#include "prune.h"
#include <vector>
#include <algorithm>

// to return multiple arguments in extract min
template<typename T>
struct triple_extract_min {
    T real_key;  // maybe corrupted but "minimum" item
    T current_key;      // minimum current_key
    std::list<T> corruption_set;
    triple_extract_min(T minVal, T key, const std::list<T> &corruptionSet) : real_key(minVal), current_key(key),
                                                                             corruption_set(corruptionSet) {}
};

// to return multiple arguments in extract min for Sorting by Witnesses
template<typename T>
struct triple_extract_min_sorting {
    T element;  // maybe corrupted but "minimum" item
    size_t corruption_set_size;      // size of corruption-set before deleting element
    std::list<T> corruption_set;
    triple_extract_min_sorting(T minVal, T key, const std::list<T> &corruptionSet) : element(minVal),
                                                                                     corruption_set_size(key),
                                                                                     corruption_set(corruptionSet) {}
};

template<typename T>
struct tuple_find_min {
    T real_key;
    T current_key;
    tuple_find_min(T realKey, T key) : real_key(realKey), current_key(key) {}
};

template<typename T>
class soft_sequence_heap {
public:
    soft_sequence_heap();

    ~soft_sequence_heap() {
        if (! this->head_list) { return; }  // soft sequence heap is empty
        head<T> *current = this->head_list;
        head<T> *current_prev = this->head_list;
        while (current != nullptr) {
            current_prev = current;
            current = current->next;
            // delete circular linked lists here (we only want to delete them if we delete a soft sequence heap)
            for (node<T> n : current_prev->sequence) {
                n.witness_set.delete_cll();
                n.corruption_set.delete_cll();
            }
            delete current_prev;
        }
    }

    std::list<T> lazily_deleted;
    int64_t rank_threshold;
    head<T> *head_list; // first head of head_list
};


template<typename T>
soft_sequence_heap<T> *make_heap(const float& eps);
template<typename T>
void insert_head_at_front(head<T> * head_list_c, head<T> *h);
template<typename T>
head<T> *merge_heads(head<T> *h1, head<T> *h2);
template<typename T>
head<T> *update_suffix_min(head<T> *h);
template<typename T>
void update_suffix_min_once(head<T> *h);
template<typename T>
head<T> *replace_heads(head<T> *first_head, head<T> *snd_head, head<T> *replace_head);
template<typename T>
void insert(soft_sequence_heap<T> *s, const T &value);
template<typename T>
void insert(soft_sequence_heap<T> *s, const std::vector<T> &values);
template<typename T>
tuple_find_min<T>* find_min(soft_sequence_heap<T> *s);
template<typename T>
circular_linked_list<T> *delete_item(soft_sequence_heap<T> *s, T item);
template<typename T>
head<T> *remove_head(soft_sequence_heap<T> *s, head<T> *h);
template<typename T>
void remove_lazily_deleted(soft_sequence_heap<T> *s, std::list<T> &corrupted_items);
template<typename T>
triple_extract_min<T> extract_min(soft_sequence_heap<T> *s);
template<typename T>
std::vector<T> *extract_all(soft_sequence_heap<T> *s);
template<typename T>
void extract_all_no_return(soft_sequence_heap<T> *s);
template<typename T>
bool empty(soft_sequence_heap<T> *s);
template<typename T>
head<T> *append_head(head<T> *h, head<T> *h1);
template<typename T>
soft_sequence_heap<T> *meld(soft_sequence_heap<T> *s1, soft_sequence_heap<T> *s2);
template<typename T>
triple_extract_min_sorting<T> extract_min_sbw(soft_sequence_heap<T> *s); // for sorting by witnesses

#include "soft_sequence_heap.tpp"


#endif //SOFT_SEQUENCE_HEAP_SOFT_HEAP_H
