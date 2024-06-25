cmake_minimum_required(VERSION 3.17)
project(SoftSequenceHeap)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-openmp")

#add_compile_options(/EHsc)
include_directories(sorting selection)

add_executable(SoftSequenceHeap
        main.cpp
        soft_sequence_heap.h
        circular_linked_list.h
        merge.h
        head.h
        node.h
        prune.h
        selection/k_selection.h
        build_ssh/build_ssh_parallel.h
        sorting/chazelle_sorting.h
        sorting/sorting_by_witnesses.h
        sorting/sorting_by_witnesses.cpp
        )