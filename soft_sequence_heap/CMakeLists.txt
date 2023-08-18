cmake_minimum_required(VERSION 3.17)
project(SoftSequenceHeap)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-openmp")

#add_compile_options(/EHsc)
include_directories(../data testing sortedness sorting ../sorting_algorithms)

add_executable(SoftSequenceHeap
        main.cpp
        soft_sequence_heap.h
        circular_linked_list.h
        merge.h
        head.h
        node.h
        prune.h
        data/generate_data.cpp
        data/generate_data.h
        data/read_data.cpp
        data/read_data.h
        selection/k_selection.h
        sortedness/inversion_count.h
        sortedness/runs.h
        sortedness/longest_ascending_subsequence.h
        sortedness/num_exchanges.h
        sortedness/rank_difference.h
        build_ssh/build_ssh_parallel.h
        sorting/chazelle_sorting.h
        sorting/sorting_by_witnesses.h
        sorting/sorting_by_witnesses.cpp
        ../sorting_algorithms/timsort.h
        ../sorting_algorithms/binaryheap.cpp
        ../sorting_algorithms/binaryheap.h
        ../sorting_algorithms/mergesort.h
        ../sorting_algorithms/quicksort.h
        testing/benchmarks.cpp
        testing/benchmarks.h
        testing/time_measurement.cpp
        testing/time_measurement.h
        testing/checksum.cpp
        testing/checksum.h
        )