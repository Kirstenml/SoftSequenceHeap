//
// Parallelized approach of building a soft sequence heap
// Idea: insert items to different soft sequence heaps and meld them together
//

#ifndef SOFTSEQUENCEHEAP_BUILD_SSH_PARALLEL_H
#define SOFTSEQUENCEHEAP_BUILD_SSH_PARALLEL_H

#include <vector>
#include <math.h>       /* ceil */
#include "../soft_sequence_heap.h"
#include <thread>


template<typename T>
void merge_meld(std::vector<soft_sequence_heap<T>*> &soft_heaps, size_t num_ssh) {
    std::vector<soft_sequence_heap<T>*> soft_heaps_res = std::vector<soft_sequence_heap<T>*>(int(ceil(double(num_ssh) / 2)));
    while (num_ssh != 1) {
        // parallelize merging
        #pragma omp parallel for
        for (int i = 0; i < num_ssh - 1; i+= 2) {
            soft_heaps_res[int(i/2)] = meld(soft_heaps[i], soft_heaps[i+1]);
        }
        if (num_ssh % 2 == 1 && num_ssh > 1) {    // odd number of soft heaps, so move the last in the row to the melded ones
            soft_heaps_res[int(ceil(double(num_ssh) / 2)) - 1] = soft_heaps[num_ssh - 1];
        }
        soft_heaps = soft_heaps_res;
        num_ssh = int(ceil(double(num_ssh) / 2));
        soft_heaps_res = std::vector<soft_sequence_heap<T>*>(int(ceil(double(num_ssh) / 2)));
    }
}


template<typename T>
soft_sequence_heap<T> *insert_meld_ssh(std::vector<T> &values, const float& eps, size_t chunk_size,
                                       bool dynamic_chunk_size = false) {
    if (dynamic_chunk_size) {
        unsigned long const hardware_threads = std::thread::hardware_concurrency();
        chunk_size = ceil(values.size() / hardware_threads);
        if (chunk_size < pow(2,5) - 1) { chunk_size = pow(2,5) - 1; }
    }
    if (values.size() < chunk_size) {
        chunk_size = values.size();
    }
    // determine number of soft heaps
    size_t num_ssh = size_t(ceil(values.size() / chunk_size));

    std::vector<soft_sequence_heap<T>*> soft_heaps = std::vector<soft_sequence_heap<T>*>(num_ssh);
    // parallelize inserting
    soft_sequence_heap<T> *s;
    #pragma omp parallel for private (s)
    for (int i = 0; i < num_ssh - 1; ++i) {
        s = make_heap<T>(eps);
        insert(s, std::vector<T>(values.begin() + (i * chunk_size), values.begin() + ((i + 1) * chunk_size)));
        soft_heaps[i] = s;
    }
    // insert last elements
    s = make_heap<T>(eps);
    insert(s, std::vector<T>(values.begin() + ((num_ssh - 1) * chunk_size), values.end()));
    soft_heaps[num_ssh - 1] = s;

    // meld all soft heaps into the first entry of soft_heaps
    merge_meld(soft_heaps, num_ssh);
    return soft_heaps[0];
}

#endif //SOFTSEQUENCEHEAP_BUILD_SSH_PARALLEL_H
