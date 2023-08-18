//
// Reduce a sequence if rank < rank_threshold and rank-rank_threshold is even
//

#ifndef SOFTSEQUENCEHEAP_PRUNE_H
#define SOFTSEQUENCEHEAP_PRUNE_H

#include <vector>
#include "node.h"
#include <cstdint>

// function receives the merged sequence
template<typename T>
void prune(std::deque<node<T>> &sequence) {
    auto sequence_res = std::deque<node<T>>();
    typename std::deque<node<T>>::iterator it;
    for (it = sequence.begin(); it != sequence.end(); ++it){
        if (it == sequence.begin() || it == (sequence.end() - 1)) { // first and last item is not pruned
            sequence_res.push_back(*it);
            continue;
        }
        // item is pruned and distributed in corruption and witness sets
        sequence_res.back().witness_set.append(it->key);
        if (! (it->witness_set.empty())) {
            sequence_res.back().witness_set.append(it->witness_set.last);
        }
        std::next(it)->corruption_set.append(it->key);   // add value of node to corruption set
        if (! (it->corruption_set.empty())) {
            sequence_res.back().corruption_set.append(it->corruption_set.last);
        }
        ++it;   // increment iterator
        sequence_res.push_back(*it);
    }
    // at this point the old sequence containing all elements including the pruned elements is deleted
    // therefore for each node its destructor is called
    std::swap(sequence, sequence_res);
}

#endif //SOFTSEQUENCEHEAP_PRUNE_H
