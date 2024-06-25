//
// Merge two heads of rank r into a new head of rank r+1
//

#ifndef SOFTSEQUENCEHEAP_MERGE_H
#define SOFTSEQUENCEHEAP_MERGE_H

#include <list>
#include "head.h"
#include "node.h"



// Merge two heads h1 and h2 into a new head h_merged using a simple variant of binary merging
// h1 and h2 were deleted when the old heads were replaced with the new one in replace_heads
template<typename T>
void binary_merge(head<T> *h1, head<T> *h2, head<T> *h_merged) {

    std::deque<node<T>> result;

    typename std::deque<node<T>>::iterator it_1 = h1->sequence.begin();
    typename std::deque<node<T>>::iterator it_2 = h2->sequence.begin();
    while (it_1 != h1->sequence.end() and it_2 != h2->sequence.end()) {
        if (it_1->key > it_2->key) {
            result.push_back(*it_2);    // creates a copy of the argument

            ++it_2;
        } else {
            result.push_back(*it_1);
            ++it_1;
        }
    }
    // if one soft sequence is not empty append remaining nodes to h
    if (it_1 != h1->sequence.end()) {
        while (it_1 != h1->sequence.end()) {
            result.push_back(*it_1);
            ++it_1;
        }
    } else if (it_2 != h2->sequence.end()) {
        while (it_2 != h2->sequence.end()) {
            result.push_back(*it_2);
            ++it_2;
        }
    }
    std::swap(h_merged->sequence, result);
}

#endif //SOFTSEQUENCEHEAP_MERGE_H
