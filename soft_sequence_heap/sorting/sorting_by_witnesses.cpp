//
// Sorting by Witnesses
// Idea: Extract all elements from the soft sequence heap and determine intervals for each corrupted element c one,
// in which c's current and real rank lie. Then select disjoint intervals and sort them (parallel).
//

#include "sorting_by_witnesses.h"



bool sort_by_first(const std::pair<size_t, size_t>& a, const std::pair<size_t, size_t>& b) {
    return (a.first < b.first);
}

// determine intervals that were covered by another interval (so remove redundant intervals)
// Interval [a,b] is covered by interval [c,d] if and only if c <= a and b <= d
void remove_intervals(std::vector<std::pair<size_t, size_t>> &intervals) {
    std::vector<std::pair<size_t, size_t>> intervals_reduced;
    // sort by start index
    std::sort(intervals.begin(), intervals.end(), sort_by_first);

    // iterate over intervals forward
    intervals_reduced.push_back(intervals[0]);
    for (int i = 1; i < intervals.size(); ++i) {
        // last interval in intervals_reduced vector covers the new interval
        if (intervals_reduced.back().first <= intervals[i].first
            and intervals_reduced.back().second >= intervals[i].second) {
            continue;
        } else {
            intervals_reduced.push_back(intervals[i]);
        }
    }
    // traverse reduced vector backwards to eliminate remaining already covered intervals
    std::vector<std::pair<size_t, size_t>> intervals_reduced_back;
    intervals_reduced_back.push_back(intervals_reduced.back());
    for (int i = intervals_reduced.size() - 2; i >= 0; --i) {
        if (intervals_reduced_back.back().first <= intervals_reduced[i].first
            and intervals_reduced_back.back().second >= intervals_reduced[i].second) {
            continue;
        } else {
            intervals_reduced_back.push_back(intervals_reduced[i]);
        }
    }
    std::reverse(intervals_reduced_back.begin(), intervals_reduced_back.end());
    intervals = intervals_reduced_back;
}


