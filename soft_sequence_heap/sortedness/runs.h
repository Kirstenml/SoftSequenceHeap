//
// Runs of a sequence S is the number of ascending substrings of S (Mannila)
//

#ifndef SOFTSEQUENCEHEAP_RUNS_H
#define SOFTSEQUENCEHEAP_RUNS_H

#include <vector>
#include <iostream>
#include <fstream>
#include "../soft_sequence_heap.h"

template<typename T>
unsigned long long runs(std::vector<T> &sequence, int n) {
    unsigned long long run = 0;
    for (int i = 0; i < n-1; ++i) {
        if (sequence[i] > sequence[i+1]) {
            ++run;
        }
    }
    return run;
}

#endif //SOFTSEQUENCEHEAP_RUNS_H
