//
// Methods to measure execution times and return each measured time for evaluations
//

#include "time_measurement.h"

unsigned long long insert_extract_interval_sorting_time(std::vector<int> &insert_vector, float eps, bool randomization,
                                                        int prob, bool parallelization) {
    soft_sequence_heap<int> *s;
    auto start = std::chrono::high_resolution_clock::now();

    long long checksum_before = std::accumulate(insert_vector.begin(), insert_vector.end(), 0);
    s = insert_meld_ssh(insert_vector, eps, pow(2, 10) - 1);
    long long checksum_after = extracting_interval_sorting_checksum(s, eps, insert_vector.size(), randomization, prob, parallelization);

    auto stop = std::chrono::high_resolution_clock::now();
    if (checksum_before != checksum_after) { std::cout << "Checksum failed in Interval Sorting \n"; }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    delete s;
    return duration.count();
}


unsigned long long insert_extract_all_time(std::vector<int> &insert_vector, float eps) {
    soft_sequence_heap<int> *s;
    auto start = std::chrono::high_resolution_clock::now();

    long long checksum_before = std::accumulate(insert_vector.begin(), insert_vector.end(), 0);
    s = insert_meld_ssh(insert_vector, eps, pow(2, 10) - 1);
    long long checksum_after = extract_all_checksum(s);

    auto stop = std::chrono::high_resolution_clock::now();
    if (checksum_before != checksum_after) { std::cout << "Checksum failed in Insert Extract \n"; }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    delete s;
    return duration.count();
}


// Build a soft sequence heap
unsigned long long meld_parallel_time(std::vector<int> &insert_vector, float eps, size_t chunk_size, bool dynamic_cs) {
    soft_sequence_heap<int> *s;
    auto start = std::chrono::high_resolution_clock::now();

    long long checksum_before = insert_vector.size();
    s = insert_meld_ssh(insert_vector, eps, chunk_size, dynamic_cs);
    long long checksum_after = highest_rank_checksum(s);

    auto stop = std::chrono::high_resolution_clock::now();
    if (checksum_after > log2(checksum_before)) { std::cout << "Checksum failed in insert-meld parallel \n"; }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    delete s;
    return duration.count();
}

unsigned long long insert_all_time(std::vector<int> &insert_vector, float eps) {
    soft_sequence_heap<int> *softHeap = make_heap<int>(eps);
    auto start = std::chrono::high_resolution_clock::now();

    long long checksum_before = insert_vector.size();
    insert(softHeap, insert_vector);
    long long checksum_after = highest_rank_checksum(softHeap);

    auto stop = std::chrono::high_resolution_clock::now();
    if (checksum_after > log2(checksum_before)) { std::cout << "Rank is too large in naive insertion. \n"; }
//    if (checksum_after > log2(checksum_before)) {throw "Checksum negative"; }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    delete softHeap;
    return duration.count();
}

unsigned long long mergesort_time(std::vector<int> &insert_vector, int j) {
    std::vector<int> insert_vector_cpy(insert_vector);
    // copy input data because mergesort sorts inplace (deep copy)
    auto start = std::chrono::high_resolution_clock::now();

    long long checksum_before = std::accumulate(insert_vector_cpy.begin(), insert_vector_cpy.end(), 0);
    mergeSort(&insert_vector_cpy[0], 0, j-1);
    long long checksum_after = std::accumulate(insert_vector_cpy.begin(), insert_vector_cpy.end(), 0);

    auto stop = std::chrono::high_resolution_clock::now();
    if (checksum_before != checksum_after) { std::cout << "Checksum failed in Mergesort \n"; }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    return duration.count();
}

unsigned long long timsort_time(std::vector<int> &insert_vector, int j) {
    std::vector<int> insert_vector_cpy(insert_vector);
    // copy input data because mergesort sorts inplace (deep copy)
    auto start = std::chrono::high_resolution_clock::now();

    long long checksum_before = std::accumulate(insert_vector_cpy.begin(), insert_vector_cpy.end(), 0);
    TimSort::timSort(&insert_vector_cpy[0], j);
    long long checksum_after = std::accumulate(insert_vector_cpy.begin(), insert_vector_cpy.end(), 0);

    auto stop = std::chrono::high_resolution_clock::now();
    if (checksum_before != checksum_after) { std::cout << "Checksum failed in Timsort \n"; }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    return duration.count();
}

unsigned long long quicksort_time(std::vector<int> &insert_vector, int j) {
    std::vector<int> insert_vector_cpy = std::vector<int>(insert_vector);
    auto start = std::chrono::high_resolution_clock::now();

    long long checksum_before = std::accumulate(insert_vector_cpy.begin(), insert_vector_cpy.end(), 0);
    quicksort(&(insert_vector_cpy[0]), 0, j - 1);
    long long checksum_after = std::accumulate(insert_vector_cpy.begin(), insert_vector_cpy.end(), 0);

    auto stop = std::chrono::high_resolution_clock::now();
    if (checksum_before != checksum_after) { std::cout << "Checksum failed in Quicksort \n"; }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    return duration.count();
}

unsigned long long binary_heap_time(std::vector<int> &insert_vector) {
    Heap h;
    auto start = std::chrono::high_resolution_clock::now();

    long long checksum_before = std::accumulate(insert_vector.begin(), insert_vector.end(), 0);
    h.makeHeap(insert_vector);
    long long checksum_after = h.heapSortChecksum();

    auto stop = std::chrono::high_resolution_clock::now();
    if (checksum_before != checksum_after) { std::cout << "Checksum failed in Heapsort \n"; }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    return duration.count();
}

unsigned long long combine_ssh_mergesort(std::vector<int> &insert_vector, int j, float eps) {
    soft_sequence_heap<int> *s;
    auto start = std::chrono::high_resolution_clock::now();

    long long checksum_before = std::accumulate(insert_vector.begin(), insert_vector.end(), 0);
    s = insert_meld_ssh(insert_vector, eps, pow(2, 10) - 1);
    std::vector<int> *extracted = extract_all(s);
    mergeSort(&((*extracted)[0]), 0, j-1);
    long long checksum_after = std::accumulate(extracted->begin(), extracted->end(), 0);

    auto stop = std::chrono::high_resolution_clock::now();
    if (checksum_before != checksum_after) { std::cout << "Checksum failed in Combined Sorting Mergesort \n"; }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    delete extracted;
    delete s;
    return duration.count();
}

unsigned long long combine_ssh_timsort(std::vector<int> &insert_vector, int j, float eps) {
    soft_sequence_heap<int> *s;
    auto start = std::chrono::high_resolution_clock::now();

    long long checksum_before = std::accumulate(insert_vector.begin(), insert_vector.end(), 0);
    s = insert_meld_ssh(insert_vector, eps, pow(2, 10) - 1);
    std::vector<int> *extracted = extract_all(s);
    TimSort::timSort(&((*extracted)[0]), j);
    long long checksum_after = std::accumulate(extracted->begin(), extracted->end(), 0);

    auto stop = std::chrono::high_resolution_clock::now();
    if (checksum_before != checksum_after) { std::cout << "Checksum failed in Combined Sorting Timsort \n"; }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    delete extracted;
    delete s;
    return duration.count();
}

unsigned long long chazelle_near_sorting_time(std::vector<int> &insert_vector, int j, float eps) {
    soft_sequence_heap<int> *s;
    auto start = std::chrono::high_resolution_clock::now();

    long long checksum_before = std::accumulate(insert_vector.begin(), insert_vector.end(), 0);
    s = insert_meld_ssh(insert_vector, eps, pow(2, 10) - 1);
    std::vector<int> *result = chazelle_sorting(s, eps, j);
    long long checksum_after = std::accumulate(result->begin(), result->end(), 0);

    auto stop = std::chrono::high_resolution_clock::now();
    if (checksum_before != checksum_after) { std::cout << "Checksum failed in Chazelle near-sorting \n"; }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    delete result;
    delete s;
    return duration.count();
}

unsigned long long std_sort_time(std::vector<int> &insert_vector) {
    auto start = std::chrono::high_resolution_clock::now();

    long long checksum_before = std::accumulate(insert_vector.begin(), insert_vector.end(), 0);
    std::sort(insert_vector.begin(), insert_vector.end());
    long long checksum_after = std::accumulate(insert_vector.begin(), insert_vector.end(), 0);

    auto stop = std::chrono::high_resolution_clock::now();
    if (checksum_before != checksum_after) { std::cout << "Checksum failed in Chazelle near-sorting \n"; }
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    return duration.count();
}