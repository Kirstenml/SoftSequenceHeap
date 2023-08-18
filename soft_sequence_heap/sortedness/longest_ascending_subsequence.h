//
// las(X) is the length of the longest ascending subsequence of X (not necessarily contiguous)
// rem(X) = n - las(X) indicates how many elements have to be removed from X to leave a sorted list
//

#ifndef SOFTSEQUENCEHEAP_LONGEST_ASCENDING_SUBSEQUENCE_H
#define SOFTSEQUENCEHEAP_LONGEST_ASCENDING_SUBSEQUENCE_H

#include <vector>
#include <iostream>
#include <fstream>
#include "../soft_sequence_heap.h"

template<typename T>
unsigned long long las(std::vector<T> &sequence, int n) {
    // m[j] stores the index k of the smallest value sequence[k] such that there is an increasing subsequence of length
    // j ending at sequence[k]
    int *m = new int[n+1];
    // p[k] stores the index of the predecessor of sequence[k] in the longest increasing subsequence ending at seq[k]
    int *p = new int[n+1];
    unsigned long long l = 0;  // length of the longest increasing subsequence found so far
    unsigned long long lo;
    unsigned long long hi;
    unsigned long long mid;

    for (int i = 0; i < n; ++i) {
        // binary search for the largest j <= l such that sequence[m[j]] < sequence[i]
        lo = 1;
        hi = l;
        while (lo <= hi) {
            mid = ceil((lo+hi)/2);
            if (sequence[m[mid]] < sequence[i]) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        // lo is 1 greater than the length of the longest prefix of sequence[i]
        int l_c = lo;

        // predecessor of sequence[i] is the last index of the subsequence of length l_c - 1
        p[i] = m[l_c - 1];
        m[l_c] = i;

        // if we found a subsequence longer than any we have found yet, update l
        if (l_c > l) {
            l = l_c;
        }
    }
    delete[] m;
    delete[] p;
    return l;
}

template<typename T>
unsigned long long rem(std::vector<T> &sequence, int n) {
    return n-las(sequence, n);
}

#endif //SOFTSEQUENCEHEAP_LONGEST_ASCENDING_SUBSEQUENCE_H
