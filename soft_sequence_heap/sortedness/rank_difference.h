//
// Given: permutation with values between 0 and n-1
// The rank difference = 1 - 2/n^2 sum |f(x) - x|
// where f(x) is the position (/rank) of an element in the permutation
// and x is the position (/rank) in the sorted permutation
//  0 for completely unsorted,
//  1 for completely sorted
//

#ifndef SOFTSEQUENCEHEAP_RANK_DIFFERENCE_H
#define SOFTSEQUENCEHEAP_RANK_DIFFERENCE_H

#include <vector>
#include <cmath>        // std::abs
#include <iostream>
#include <fstream>
#include "../soft_sequence_heap.h"

template<typename T>
long double rank_diff(std::vector<T> *permutation) {
    int n = permutation->size();
    if (n == 0) { return 0; }
    int sum = abs((*permutation)[0]);
    for (int i = 1; i < n; ++i) {
        sum += abs((*permutation)[i] - i);
    }
    long double subtract = (2 / pow(n, 2)) * sum;
    return 1 - subtract;
}

#endif //SOFTSEQUENCEHEAP_RANK_DIFFERENCE_H
