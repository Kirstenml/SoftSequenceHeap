# Exact Sorting Using the Approximate Priority Queue Soft Sequence Heaps
## Implementations in C++ of the Master's Thesis by Kirsten Köbschall

This project is structured as follows:

### Soft sequence heaps

This folder include 

* Build soft sequence heap - Parallelized approach to insert elements to a soft sequence heap
* Data - Generate and read data from csv
* Selection - Chazelle's selection algorithm to select the *k*th smallest element using a soft sequence heap
* Sortedness - Metrics to measure how sorted a given sequence is
* Sorting 
	* Chazelle's near-sorting approach
	* Sorting by Witnesses
* Testing 
	* Benchmarks - Write benchmarks to csv data for plotting
	* Checksum - functions to guarantee that no compiler optimization happens during time measurements
	* Time measurement - Measuring time for each presented sorting approach
	
and all components of soft sequence heaps:
* Circular linked lists (for corruption- and witness-sets)
* Head of a soft sequence
* Merge two soft sequences
* Node in a soft sequence
* Prune function
* Soft sequence heap methods

### Sorting algorithms

Here are the implementations of all sorting algorithms used to evaluate the above algorithms:

* (Binary) Heapsort
* Mergesort
* Quicksort
* Timsort