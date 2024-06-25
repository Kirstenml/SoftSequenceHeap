# Exact Sorting Using the Approximate Priority Queue Soft Sequence Heaps

## Overview
This project contains an implementation in C++ of soft sequence heaps (SSH) introduced by G. Brodal (2021). 
Based on SSH we introduce a new sorting algorithm *Sorting by Witnesses* which requires less comparisons than Quicksort 
or Timsort on random data.
This work is a master's thesis von Kirsten K..

## Implementation
* Soft Sequence Heaps
* Sorting by Witnesses

## Installation
* C++ compiler: MinGW 11.0
* CMake: 3.28.1

## Example Code
```c++
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
```

# Abstract of my Master's Thesis
Soft heaps were introduced by Chazelle as a simple variant of a priority queue in 1998. 
Since soft heaps trade accuracy for speed, elements are allowed to increase their key to travel in packets, therefore, 
they are considered as corrupted. In 2021, Brodal introduced soft sequence heaps as an alternative implementation of 
soft heaps using sorted sequences to store items and analogous time bounds to those of Chazelle’s soft heaps. 
In this work Sorting by Witnesses is introduced as a new sorting approach using a soft sequence heap. 
Since soft sequence heaps have an astonishingly low number of comparisons for inserting *n* elements and subsequent 
extractions, Sorting by Witnesses with small $\epsilon$ values requires less comparisons than Quicksort or 
Timsort for random data. Sequence Heapsort inserts elements to a non-soft sequence heap and extracts all of them. 
This approach performs as fast as Heapsort but requires less comparisons. A modified near-sorting approach of Chazelle 
requires the smallest number of comparisons for sorting exactly.

**Literature**: Brodal, Gerth. (2021). Soft Sequence Heaps. 10.1137/1.9781611976496.2. 