//
// Methods to measure execution times and return each measured time for evaluations
//

#ifndef SOFTSEQUENCEHEAP_TIME_MEASUREMENT_H
#define SOFTSEQUENCEHEAP_TIME_MEASUREMENT_H

#include <vector>
#include <iostream>
#include <fstream>
#include <numeric>
#include <algorithm>
#include "../soft_sequence_heap.h"
#include <chrono>   // time measurement
#include "../../sorting_algorithms/quicksort.h"
#include "checksum.h"
#include "../../sorting_algorithms/timsort.h"
#include "../sorting/sorting_by_witnesses.h"
#include "../../sorting_algorithms/binaryheap.h"
#include "../sorting/chazelle_sorting.h"
#include "../build_ssh/build_ssh_parallel.h"
#include "../../sorting_algorithms/mergesort.h"


unsigned long long insert_extract_interval_sorting_time(std::vector<int> &insert_vector, float eps, bool randomization,
                                                        int prob, bool parallelization);
unsigned long long insert_extract_all_time(std::vector<int> &insert_vector, float eps);
unsigned long long meld_parallel_time(std::vector<int> &insert_vector, float eps, size_t chunk_size,
                                      bool dynamic_cs = false);
unsigned long long insert_all_time(std::vector<int> &insert_vector, float eps);
unsigned long long mergesort_time(std::vector<int> &insert_vector, int j);
unsigned long long timsort_time(std::vector<int> &insert_vector, int j);
unsigned long long quicksort_time(std::vector<int> &insert_vector, int j);
unsigned long long binary_heap_time(std::vector<int> &insert_vector);
unsigned long long combine_ssh_mergesort(std::vector<int> &insert_vector, int j, float eps);
unsigned long long combine_ssh_timsort(std::vector<int> &insert_vector, int j, float eps);
unsigned long long chazelle_near_sorting_time(std::vector<int> &insert_vector, int j, float eps);
unsigned long long std_sort_time(std::vector<int> &insert_vector);

#endif //SOFTSEQUENCEHEAP_TIME_MEASUREMENT_H
