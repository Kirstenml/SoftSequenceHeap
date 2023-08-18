//
// Generate Data for testing and analyzing all presented (in my Master's Thesis) sorting algorithms
//

#ifndef SOFTSEQUENCEHEAP_GENERATE_DATA_H
#define SOFTSEQUENCEHEAP_GENERATE_DATA_H

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <random>
#include <vector>

int *sort_partially(const int n, const int prob);
int *fisher_yates_shuffle(const int *arr, int n);
void write_permutations(const int *len, const int num, const int disorder, bool large = false, const int prob = 65);
void generate_data(int order_permutations, bool large, int prob = 65);

#endif //SOFTSEQUENCEHEAP_GENERATE_DATA_H