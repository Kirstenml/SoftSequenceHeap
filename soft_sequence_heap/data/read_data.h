//
// Read test data from txt/csv files
//

#ifndef SOFTSEQUENCEHEAP_READ_DATA_H
#define SOFTSEQUENCEHEAP_READ_DATA_H

std::vector<std::vector<int>> read_data(std::string filename = "random_permutations");
std::vector<std::vector<int>> read_file(int order_permutations, int num, bool large);

#endif //SOFTSEQUENCEHEAP_READ_DATA_H
