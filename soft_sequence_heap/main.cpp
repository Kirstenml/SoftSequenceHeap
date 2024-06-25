#include <iostream>
#include "soft_sequence_heap.h"
#include <vector>
#include "sorting/sorting_by_witnesses.h"
#include "build_ssh/build_ssh_parallel.h"


int main() {
    float eps = 0.8;
    soft_sequence_heap<int> *softSequenceHeap = make_heap<int>(eps);

    std::vector<int> numbers{13, 5, 30, 20, 50, 14, 26, 33, 21, 4, 17, 44, 23, 9, 11};
    insert(softSequenceHeap, numbers);
    std::cout << *softSequenceHeap << std::endl;
    // Extract approximate minimum
    std::cout << extract_min(softSequenceHeap) << std::endl;
    // Delete an item
    delete_item(softSequenceHeap, 26);
    std::cout << *softSequenceHeap << std::endl;
    // Extract all items
    extract_all(softSequenceHeap);

    // Meld two soft sequence heaps
    soft_sequence_heap<int> *softSequenceHeap2 = make_heap<int>(eps);
    std::vector<int> numbers2{1, 2, 6, 7, 23, 7, 18, 16, 70, 66, 3};
    insert(softSequenceHeap2, numbers2);
    soft_sequence_heap<int> *s = meld(softSequenceHeap, softSequenceHeap2);
    std::cout << *s << std::endl;

    // Fast approach to create a soft sequence heap
    std::vector<int> insert_vector{13, 5, 30, 20, 50, 14, 26, 33, 21, 4, 17, 44, 23, 9};
    size_t chunk_size = 2;
    bool dynamic_cs = false;
    soft_sequence_heap<int> *sMelded = insert_meld_ssh(insert_vector, eps, chunk_size, dynamic_cs);
    std::cout << *sMelded << std::endl;

    // Our contribution: Sorting by witnesses
    std::vector<int> values_res = *extracting_interval_sorting(sMelded, eps, insert_vector.size());
    for (int i: values_res)
        std::cout << i << ' ';


    return 0;
}
