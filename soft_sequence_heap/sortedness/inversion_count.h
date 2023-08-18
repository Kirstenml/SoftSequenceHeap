//
// Inversion count = Number of paris in wrong order
//

#ifndef SOFTSEQUENCEHEAP_INVERSION_COUNT_H
#define SOFTSEQUENCEHEAP_INVERSION_COUNT_H

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdint>
#include "inversion_count.h"
#include <string>
#include "../soft_sequence_heap.h"
#include <algorithm>

// Inversion count in O(n log n) - modification of Mergesort
namespace inversion_count {

    // merge two sorted arrays and returns inversion count in the arrays
    template<typename T>
    unsigned long long merge(std::vector<T> &arr, std::vector<T> &temp, size_t left, size_t mid, size_t right) {
        size_t i, j, k;
        unsigned long long inv_count = 0;

        i = left; // i is index for left subarray
        j = mid; // j is index for right subarray
        k = left; // k is index for resultant merged subarray
        while ((i <= mid - 1) && (j <= right)) {
            if (arr[i] <= arr[j]) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
                inv_count = inv_count + (mid - i);
            }
        }
        while (i <= mid - 1) { temp[k++] = arr[i++]; }
        while (j <= right) { temp[k++] = arr[j++]; }
        for (i = left; i <= right; i++) { arr[i] = temp[i]; }
        return inv_count;
    }

    // An auxiliary recursive function that sorts the input array and returns the number of inversions in the array
    template<typename T>
    unsigned long long inv_count_mergesort(std::vector<T> &arr, std::vector<T> &temp, size_t left, size_t right) {
        size_t mid = 0;
        unsigned long long inv_count = 0;
        if (right > left) {
            mid = (right + left) / 2;
            // left and right part
            inv_count += inversion_count::inv_count_mergesort(arr, temp, left, mid);
            inv_count += inversion_count::inv_count_mergesort(arr, temp, mid + 1, right);
            // merge the two parts
            inv_count += inversion_count::merge(arr, temp, left, mid + 1, right);
        }
        return inv_count;
    }

    template<typename T>
    unsigned long long inv_count(std::vector<T> &arr, size_t n) {
        std::vector<T> temp = std::vector<T>(n);
        std::vector<T> arr_cpy(arr);
        return inversion_count::inv_count_mergesort(arr_cpy, temp, 0, n - 1);
    }

}

#endif //SOFTSEQUENCEHEAP_INVERSION_COUNT_H
