//
// Modified near-sorting (using Chazelle's near-sorting approach)
// Idea: Choose a minima x_i of each 2*eps*n extracted elements, where x_i is at the beginning of T_i, the time interval
// uncorrupted and set them as interval bounds. Sort all elements into the disjoint intervals and sort them in
// parallel using Timsort
//

#ifndef SOFTSEQUENCEHEAP_CHAZELLE_SORTING_H
#define SOFTSEQUENCEHEAP_CHAZELLE_SORTING_H

#include "../soft_sequence_heap.h"
#include "../sorting_algorithms/timsort.h"
#include <numeric>  //accumulate

extern unsigned long long num_comparisons_chazelle_sorting;

// to measure the number of comparisons - parts are commented out in find_minima
template<typename T>
bool find_in_list(std::list<T> &corrupted_in_chunk, T val);

// auxiliary function to extract elements from ssh and find minima for interval bounds
template<typename T>
void find_minima(soft_sequence_heap<T> *ssh, float eps, size_t n, std::vector<T> &minima, std::vector<T> &extracted_elements) {
    size_t chunk_size = ceil(2*eps*n);
    size_t l = ceil(float(n)/chunk_size);
    T min_val;
    // determine minima
    for (int i = 0; i < l - 1; ++i) {
        {
            // elements which become corrupted during the extracting in the current chunk (Chazelle: U_i)
            std::list<T> corrupted_in_chunk;
            triple_extract_min<T> extracted = extract_min(ssh);
            extracted_elements.push_back(extracted.real_key);
            min_val = extracted.current_key;
            for (int j = 1; j < chunk_size; ++j) {
                triple_extract_min<T> extracted = extract_min(ssh);
                extracted_elements.push_back(extracted.real_key);
                // Note: x_i could be corrupted but it has been in a non-corrupted state at the beginning of T_i
                if (extracted.real_key != extracted.current_key) {  // the item has to be corrupted
                    // if the real key of the extracted item is corrupted during the current time interval, it could be x_i
                    if ((std::find(corrupted_in_chunk.begin(), corrupted_in_chunk.end(), extracted.real_key) !=corrupted_in_chunk.end())) {
                        min_val = std::min(min_val, extracted.real_key);
                    }
                    // only to measure number of comparisons!
//                    if (find_in_list(corrupted_in_chunk, extracted.real_key)) {
//                        min_val = std::min(min_val, extracted.real_key);
//                    }
                    corrupted_in_chunk.insert(corrupted_in_chunk.end(),
                                              extracted.corruption_set.begin(), extracted.corruption_set.end());
                } else {
                    min_val = std::min(min_val, extracted.real_key);
                }
            }
        }
        minima[i] = min_val;
    }
    // last elements in ssh (at most chunk_size elements)
    {
        std::list<T> corrupted_in_chunk;
        triple_extract_min<T> extracted = extract_min(ssh);
        extracted_elements.push_back(extracted.real_key);
        min_val = extracted.current_key;
        while (! empty(ssh)) {
            triple_extract_min<T> extracted = extract_min(ssh);
            extracted_elements.push_back(extracted.real_key);
            // Note: x_i could be corrupted but it has been in a non-corrupted state at the beginning of T_i
            if (extracted.real_key != extracted.current_key) {  // the item has to be corrupted
                // if the real key of the extracted item is corrupted during the current time interval, it could be x_i
                if ((std::find(corrupted_in_chunk.begin(), corrupted_in_chunk.end(), extracted.real_key) !=corrupted_in_chunk.end())) {
                    min_val = std::min(min_val, extracted.real_key);
                }
                // only to measure number of comparisons!
//                if (find_in_list(corrupted_in_chunk, extracted.real_key)) {
//                    min_val = std::min(min_val, extracted.real_key);
//                }
                corrupted_in_chunk.insert(corrupted_in_chunk.end(),
                                          extracted.corruption_set.begin(), extracted.corruption_set.end());
            } else {
                min_val = std::min(min_val, extracted.real_key);
            }
        }
    }
    minima[l-1] = min_val;
    num_comparisons_chazelle_sorting += num_comparisons;    // num comparisons of building and extracting from SSH
}


// Chazelle's near-sorting approach and parallel sorting of the selected intervals
template<typename T>
std::vector<T> *chazelle_sorting(soft_sequence_heap<T> *ssh, float eps, size_t n) {
    std::vector<T> extracted_elements;
    size_t chunk_size = ceil(2*eps*n);
    size_t l = ceil(float(n)/chunk_size);
    std::vector<T> minima(l);
    // extract elements and determine minima
    find_minima(ssh, eps, n, minima, extracted_elements);
    // add elements into the interval bounds to get distinct intervals
    std::vector<std::vector<int>> distinct_intervals(l);
    int i = 0;
    size_t chunk_index = 1;
    float num_of_interval_checks = 0;   // if elements does not lay directly in their interval
    for (int val : extracted_elements) {
        // check in which interval val lies
        for (int k = chunk_index; k >= 0; --k) {
            if (k  == l) {   // elements which belong in the last interval
                ++num_comparisons_chazelle_sorting;
                if (val >= minima[l - 1]) {
                    distinct_intervals[l - 1].push_back(val);
                    break;
                }
                continue;
            }
            ++num_comparisons_chazelle_sorting;
            if (val >= minima[k]) {
                distinct_intervals[k].push_back(val);
                break;
            }
            ++num_of_interval_checks;
        }
        ++i;
        if (i%chunk_size == 0) { ++chunk_index; }
    }

    // sort each of the l intervals in parallel (bc they are distinct!)
    #pragma omp parallel for
    for (int i = 0; i < l; ++i) {
        TimSort::timSort(&distinct_intervals[i][0], distinct_intervals[i].size());
    }
    num_comparisons_chazelle_sorting += num_comparison_timsort;
    // append sorted vectors
    auto *result = new std::vector<T>(distinct_intervals[0]);
    for (int i = 1; i < l; ++i) {
        result->insert(result->end(), distinct_intervals[i].begin(), distinct_intervals[i].end());
    }
    // rho = num_of_interval_checks/float(num_corrupted_before_time_interval
    return result;
}

template<typename T>
bool find_in_list(std::list<T> &corrupted_in_chunk, T val) {
    bool found = false;
    for (T c : corrupted_in_chunk) {
        ++num_comparisons_chazelle_sorting;
        if (c == val) {
            found = true;
            break;
        }
    }
    return found;
}

#endif //SOFTSEQUENCEHEAP_CHAZELLE_SORTING_H
