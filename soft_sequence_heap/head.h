//
// A head stores
// - a pointer to the soft sequence (sequence) where the data is stored
// - suffix_min pointer which points to the head with rank greater or equal with smallest first current_key
// - rank
// - a pointer to the next (with greater rank) or previous (prev) (with smaller rank) head in the head-list
//

#ifndef SOFTSEQUENCEHEAP_HEAD_H
#define SOFTSEQUENCEHEAP_HEAD_H

#include "node.h"
#include <deque>

template<typename T>
struct head{
    explicit head(node<T> n) {
        this->sequence = std::deque<node<T>>{n};
        this->suffix_min = nullptr;
        this->rank = 0;
        this->next = nullptr;
        this->prev = nullptr;
    }
    head() {
        this->suffix_min = nullptr;
        this->rank = 0;
        this->next = nullptr;
        this->prev = nullptr;
    }

    ~head() {
        this->suffix_min = nullptr;
        this->next = nullptr;
        this->prev = nullptr;
    }

    std::deque<node<T>> sequence;
    head<T> *suffix_min;   //points to the root of minimum current_key among all roots with larger or equal rank
    size_t rank;
    head<T> *next;
    head<T> *prev;

};


template<typename T>
std::ostream& operator<<(std::ostream &out, head<T> const& h) {
    out << "rank " << h.rank << ": [" << std::endl;
    int i = 0;
    for (auto const &n: h.sequence) {
        ++i;
        out << n.key << " (C=" << n.corruption_set << ", W=" << n.witness_set << ")";
        if (i != h.sequence.size()) { out << ", "; }
        out << std::endl;
    }
    out << "]" << std::endl;
    return out;
}


#endif //SOFTSEQUENCEHEAP_HEAD_H
