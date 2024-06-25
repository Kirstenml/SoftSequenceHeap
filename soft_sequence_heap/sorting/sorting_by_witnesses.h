//
// Sorting by Witnesses
// Idea: Extract all elements from the soft sequence heap and determine intervals for each corrupted element c one,
// in which c's current and real rank lie. Then select disjoint intervals and sort them (parallel).
//

#ifndef SOFTSEQUENCEHEAP_SORTING_BY_WITNESSES_H
#define SOFTSEQUENCEHEAP_SORTING_BY_WITNESSES_H

#include <vector>
#include <deque>
#include <algorithm>
#include <map>
#include "../soft_sequence_heap.h"
#include <numeric>  //accumulate

bool sort_by_first(const std::pair<size_t, size_t>& a, const std::pair<size_t, size_t>& b);
void remove_intervals(std::vector<std::pair<size_t, size_t>> &intervals);

template<typename T>
struct tuple_extract_partially{
    std::vector<std::pair<size_t, size_t>> intervals;
    std::vector<T> values;

    tuple_extract_partially(const std::vector<std::pair<size_t, size_t>> &intervals, const std::vector<T> &values)
            : intervals(intervals), values(values) {};
};

// Returns the determined intervals and presorted values by extracting elements from the soft sequence heap
template<typename T>
tuple_extract_partially<T> *presorted_intervals(soft_sequence_heap<T> *s, float eps, size_t n, bool randomization = false, int prob = 75) {
    std::vector<T> sorted = std::vector<T>();
    std::map<T,size_t> corrupted;
    size_t idx = 0;
    size_t idx_begin;
    srand((unsigned) time(0));
    std::vector<std::pair<size_t, size_t>> intervals;

    while (!empty(s)) {  // extract all
        {
            triple_extract_min_sorting<T> extracted = extract_min_sbw(s);

            // store the currently corrupted items with their index in the sorted list to determine an interval
            if (!extracted.corruption_set.empty()) {
                for (T v : extracted.corruption_set) {
                    corrupted.insert(std::pair<T, int>(v, idx));
                }
            }
            sorted.push_back(extracted.element);
            ++idx;

            // there must be a witness in corrupted if the current element is corrupted
            // if corruption_set_size is not zero, the current element was part of a corruption-set
            // and therefore corrupted
            if ((extracted.corruption_set_size != 0) and
                ((randomization and (rand() % 100) < prob) or !randomization)) {
                idx_begin = corrupted[extracted.element];
                // index + eps*n + |corruption-set|
                intervals.emplace_back(idx_begin, std::min(int(n-1), int(float(idx) + eps*n + extracted.corruption_set_size)));
//                std::cout << extracted.real_key << ": [" << idx_begin << "," << std::min(int(n-1), int(float(idx) + eps*n)) << "], ";
            }
        }
    }
    return new tuple_extract_partially<T>(intervals, sorted);
}

// Select the determined intervals and sort them using insertion sort
template<typename T>
void sort_selected_intervals(tuple_extract_partially<T> *t_extract) {
    // at the moment it is not possible to parallelize the sorting, because the intervals still overlap.
    for (int i = 0; i < t_extract->intervals.size(); ++i) {
        // TimSort::timSort(&(t_extract->values[t_extract->intervals[i].first]),
        //               t_extract->intervals[i].second - t_extract->intervals[i].first);
        std::sort(t_extract->values.begin() + t_extract->intervals[i].first,
                  t_extract->values.begin() + ( t_extract->intervals[i].second - t_extract->intervals[i].first ));
    }
}

// Select the distinct intervals and sort them in parallel using insertion sort
template<typename T>
void sort_selected_intervals_parallel(tuple_extract_partially<T> *t_extract) {
    // recursive idea
    if (t_extract->intervals.size() == 0) { return; }
    // select distinct intervals to sort in parallel and remove them from the remaining intervals
    std::vector<std::pair<size_t, size_t>> distinct_intervals;
    std::vector<std::pair<size_t, size_t>> remaining_intervals;
    distinct_intervals.push_back(t_extract->intervals.front());
    for (int i = 1; i < t_extract->intervals.size(); ++i) {
        if (distinct_intervals.back().second < t_extract->intervals[i].first) {
            distinct_intervals.push_back(t_extract->intervals[i]);
        } else {
            remaining_intervals.push_back(t_extract->intervals[i]);
        }
    }
    #pragma omp parallel for
    for (int i = 0; i < distinct_intervals.size(); ++i) {
        // we use Timsort to get the number of comparisons
        std::sort(t_extract->values.begin() + distinct_intervals[i].first,
                  t_extract->values.begin() + ( distinct_intervals[i].second - distinct_intervals[i].first ));
        // TimSort::timSort(&(t_extract->values[distinct_intervals[i].first]),
        //               distinct_intervals[i].second - distinct_intervals[i].first);
    }
    t_extract->intervals = remaining_intervals;
    sort_selected_intervals_parallel(t_extract);
}

// Sorting by Witnesses - Main algorithm
template<typename T>
std::vector<T> *extracting_interval_sorting(soft_sequence_heap<T> *s, float eps, size_t n, bool randomization = false,
                                            int prob = 75, bool parallelize = false) {
    tuple_extract_partially<T> *tuple_result = presorted_intervals(s, eps, n, randomization, prob);
    if (! tuple_result->intervals.empty()) {  // corruption
        remove_intervals(tuple_result->intervals);
        if (parallelize) {
            sort_selected_intervals_parallel(tuple_result);
        } else {
            sort_selected_intervals(tuple_result);
        }
    }
    std::vector<T> *values_res = new std::vector<T>(tuple_result->values);
    delete tuple_result;
    return values_res;
}

/////// ------TESTING------

template<typename T>
long long extracting_interval_sorting_checksum(soft_sequence_heap<T> *s, float eps, size_t n,
                                               bool randomization = false, int prob = 75, bool parallelize = false) {
    tuple_extract_partially<T> *tuple_result = presorted_intervals(s, eps, n, randomization, prob);
    if (! tuple_result->intervals.empty()) {  // corruption
        remove_intervals(tuple_result->intervals);
        if (parallelize) {
            sort_selected_intervals_parallel(tuple_result);
        } else {
            sort_selected_intervals(tuple_result);
        }
    }
    long long checksum = std::accumulate(tuple_result->values.begin(), tuple_result->values.end(), 0);
    delete tuple_result;
    return checksum;
}

/////// PRINTING
template<typename T>
std::ostream& operator<<(std::ostream &out, tuple_extract_partially<T> const& tupleExtractPartially) {
    out << "Presorted values: ";
    for (T v : tupleExtractPartially.values) {
        out << v << ", ";
    }
    out << "\n";
    out << "Intervals: ";
    for (std::pair<size_t, size_t> p : tupleExtractPartially.intervals) {
        out << "[" << p.first << "," << p.second << "], ";
    }
    out << "\n";
    return out;
}

#include "sorting_by_witnesses.cpp"

#endif //SOFTSEQUENCEHEAP_SORTING_BY_WITNESSES_H
