//
// Read test data from txt/csv files
//
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream

std::vector<std::vector<int>> read_data(std::string filename) {
    std::ifstream myfile;
    auto result = std::vector<std::vector<int>>();
    myfile.open("../data/" + filename + ".txt");
    if(!myfile.is_open()) throw std::runtime_error("Could not open file");
    std::string line;
    int val;
    // Read data, line by line
    while(std::getline(myfile, line)) {
        // Create a string stream of the current line
        std::stringstream ss(line);
        std::vector<int> row;
        // Extract each integer value val
        while(ss >> val){
            row.push_back(val);
            // If the next token is a comma, ignore it and move on
            if(ss.peek() == ',') ss.ignore();
        }
        row.shrink_to_fit();
        result.push_back(row);
    }
    myfile.close();
    result.shrink_to_fit();
    return result;
}

std::vector<std::vector<int>> read_file(int order_permutations, int num, bool large) {
    std::vector<std::vector<int>> test_data;
    if (order_permutations == 0) {
        test_data = std::vector<std::vector<int>>();
        for (int i = 0; i < num; ++i) {
            std::vector<int> sorted_vector((i+1)*200);
            for (int j = 0; j < (i+1)*200; ++j) {
                sorted_vector[j] = j;
            }
            test_data.push_back(sorted_vector);
        }
    } else if (order_permutations == 1) {
        if (! large) { test_data = read_data("partially_sorted"); }
        else { test_data = read_data("partially_sorted_large"); }
    } else {
        if (! large) { test_data = read_data("random_permutations"); }
        else { test_data = read_data("random_permutations_large"); }
    }
    return test_data;
}
