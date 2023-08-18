//
// Measure execution times and number of comparisons of all presented (in my Master's Thesis) sorting algorithms
//

#ifndef SOFTSEQUENCEHEAP_BENCHMARKS_H
#define SOFTSEQUENCEHEAP_BENCHMARKS_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include <iomanip> // set precision
#include "time_measurement.h"
#include "../sortedness/inversion_count.h"
#include "../sortedness/rank_difference.h"
#include "../sortedness/longest_ascending_subsequence.h"
#include "../sortedness/runs.h"
#include "../sortedness/num_exchanges.h"

// write benchmarks to file
// order_permutations = 1 then we use the partially sorted data (p=65)
// order_permutations = 2 then we use the random permutations
void write_benchmarks(char order_permutations);

#endif //SOFTSEQUENCEHEAP_BENCHMARKS_H
