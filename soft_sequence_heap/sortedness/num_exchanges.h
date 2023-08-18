//
// exc(X) = the smallest number of exchanges of arbitrary elements needed to bring X into ascending order (Mannila)
// source: https://www.geeksforgeeks.org/minimum-number-swaps-required-sort-array/
//

#ifndef SOFTSEQUENCEHEAP_NUM_EXCHANGES_H
#define SOFTSEQUENCEHEAP_NUM_EXCHANGES_H

#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "../soft_sequence_heap.h"

template<typename T>
unsigned long long exc(std::vector<T> &vec, size_t n) {
    unsigned long long res = 0;
    std::vector<T> arr = std::vector<T>(vec);
    std::vector<T> temp = std::vector<T>(arr);

    // Hashmap which stores the indices of the input array
    std::map <T, size_t> h;

    std::sort(temp.begin(), temp.end());
    for (size_t i = 0; i < n; ++i) {
        h[arr[i]] = i;
    }
    for (size_t i = 0; i < n; ++i) {
        // This is checking whether the current element is at the right place or not
        if (arr[i] != temp[i]) {
            ++res;
            T init = arr[i];
            // If not, swap this element with the index of the element which should come here
            std::swap(arr[i], arr[h[temp[i]]]);
            // Update the indexes in the hashmap accordingly
            h[init] = h[temp[i]];
            h[temp[i]] = i;
        }
    }
    return res;
}

#endif //SOFTSEQUENCEHEAP_NUM_EXCHANGES_H
