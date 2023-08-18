//
// Select the kth smallest item of n items in O(n) time using a soft sequence heap (Chazelle's approach)
//

#ifndef SOFTSEQUENCEHEAP_K_SELECTION_H
#define SOFTSEQUENCEHEAP_K_SELECTION_H

#include "../soft_sequence_heap.h"

template<typename T>
size_t partition_selection(std::vector<T> &elements, T e) {
    size_t i = -1;  // Index of smaller element
    size_t n = elements.size();
    for (size_t j = 0; j < n; ++j) {
        // If current element is smaller than or equal to pivot
        if (elements[j] <= e) {
            ++i;    // increment index of smaller element
            swap(&elements[i], &elements[j]);
        }
    }
    swap(&elements[i + 1], &elements[n-1]);
    return (i + 1);
}

template<typename T>
T selection(std::vector<T> *elements, size_t n, size_t k, float eps) {
    soft_sequence_heap<T> *s = make_heap<T>(eps);
    insert(s, *elements);
    std::vector<T> *extracted = new std::vector<T>();

    // determine the largest real current_key e
    T e_extract = extract_min(s).real_key;
    T e = e_extract;
    extracted->push_back(e_extract);
    for (int i = 1; i < eps*n; ++i) {
        e_extract = extract_min(s).real_key;
        extracted->push_back(e_extract);
        if (e < e_extract) { e = e_extract; }
    }

    // determine the real rank of e (rank is between 0 and n-1)
    // we know: e has rank at least eps*n, bc we extracted eps*n elements which are definitely smaller than e
    // Quicksorts partition
    size_t r = partition_selection(*elements, e);

    std::vector<T> *elements_reduced;
    if (r == k) { // we are done
        delete extracted;
        delete s;
        return e;
    } else if (r > k) { // remove all elements larger than e
        // so choose the front part of elements
        elements_reduced = new std::vector<T>(elements->begin(), elements->begin() + r);
    } else {    // r < k we are looking for an item larger than e
        // remove all elements smaller than e
        elements_reduced = new std::vector<T>(elements->begin() + r, elements->end());
        k = k - r;
    }
    delete extracted;
    delete s;
    selection(elements_reduced, elements_reduced->size(), k, eps);
}

template<typename T>
void kth_selection(std::vector<T> *
        elements, size_t n, size_t k, float eps = pow(3, -1)) {
    if (k == 1) {
        // select minimum
    } else if (k == n) {
        // select maximum
    } else {
        T res = selection(elements, n, k, eps);
    }
}



#endif //SOFTSEQUENCEHEAP_K_SELECTION_H
