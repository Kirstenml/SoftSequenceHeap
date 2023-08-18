//
// Measure execution times and number of comparisons of all presented (in my Master's Thesis) sorting algorithms
//

#include "benchmarks.h"

unsigned long long num_comparisons = 0;
unsigned long long num_comparisons_interval_sorting = 0;
unsigned long long num_comp_binary_heapsort = 0;
unsigned long long num_comparisons_insertion_sort = 0;
unsigned long long num_comp_merge_sort = 0;
unsigned long long num_comp_quicksort = 0;
unsigned long long num_comparison_timsort = 0;
unsigned long long num_comparisons_chazelle_sorting = 0;

void write_benchmarks(char order_permutations) {
    // read file
    std::ifstream myfile;
    auto result = std::vector<std::vector<int>>();
    if (order_permutations == 2) {
        myfile.open("../data/random_permutations_large.txt");
    } else if (order_permutations == 1) {
        myfile.open("../data/partially_sorted_large.txt");
    }
    if(!myfile.is_open()) throw std::runtime_error("Could not open file");
    std::string line;
    int val;
    // write file
    std::ofstream writefile;
    if (order_permutations == 2) {
        writefile.open("benchmarks_random.csv");
    } else if (order_permutations == 1) {
        writefile.open("benchmarks_partially_sorted.csv");
    }
    writefile << "seq_len" << ","
              // 1. Build SSH
              << "parallel_insert_meld_28-1_ms" << ","
              << "parallel_insert_meld_210-1_ms" << ","
              << "parallel_insert_meld_dynamic_ms" << ","
              << "insert_to_ssh_0.1_ms" << ","
              // 2. Comparisons
              // SSH eps=0.1
              << "comparisons_insert_extract_ssh_0.1" << ","
              << "inv_count_ssh_0.1" << ","
              << "rem_ssh_0.1" << ","
              << "runs_ssh_0.1" << ","
              << "rank_diff_ssh_0.1" << ","
              << "num_exchanges_0.1" << ","
              // SSH eps=0.01
              << "comparisons_insert_extract_ssh_0.01" << ","
              << "inv_count_ssh_0.01" << ","
              << "rem_ssh_0.01" << ","
              << "runs_ssh_0.01" << ","
              << "rank_diff_ssh_0.01" << ","
              << "num_exchanges_0.01" << ","
              // SSH eps=0.001
              << "comparisons_insert_extract_ssh_0.001" << ","
              << "inv_count_ssh_0.001" << ","
              << "rem_ssh_0.001" << ","
              << "runs_ssh_0.001" << ","
              << "rank_diff_ssh_0.001" << ","
              << "num_exchanges_0.001" << ","
              // Correct Sorting - Interval Sorting
              << "sorting_by_witnesses_0.01_ms" << ","
              << "comparisons_sorting_by_witnesses_0.01" << ","
              << "sorting_by_witnesses_0.001_ms" << ","
              << "comparisons_sorting_by_witnesses_0.001" << ","
              << "sorting_by_witnesses_0.0001_ms" << ","
              << "comparisons_sorting_by_witnesses_0.0001" << ","
              << "sequence_heapsort_ms" << ","
              << "comparisons_sequence_heapsort" << ","
              // Sorting Algorithms
              << "quicksort_ms" << ","
              << "comparisons_quicksort" << ","
              << "mergesort_ms" << ","
              << "comparisons_mergesort" << ","
              << "timsort_ms" << ","
              << "comparisons_timsort" << ","
              << "binary_heapsort_ms" << ","
              << "comparisons_heapsort" << ","
              << "std_sort_ms" << ","
              // combined sorting
              << "combine_mergesort_0.001" << ","
              << "comparisons_combine_mergesort_0.001" << ","
              << "combine_timsort_0.001" << ","
              << "comparisons_combine_timsort_0.001" << ","
              // near sorting approach Chazelle
              << "mod_near_sorting_0.001_ms"
              //<< "comparisons_mod_near_sorting_0.001"
              << "\n";
    int j;
    // Read data, line by line
    while(std::getline(myfile, line)) {
        // Create a string stream of the current line
        std::stringstream ss(line);
        {
            std::vector<int> row;
            // Extract each integer value val
            while (ss >> val) {
                row.push_back(val);
                // If the next token is a comma, ignore it and move on
                if (ss.peek() == ',') ss.ignore();
            }
            row.shrink_to_fit();

            // perform experiments on vector row
            j = row.size();
            std::cout << "j = " << j << ": \n";
            writefile << j << ",";

            // BUILD SOFT SEQUENCE HEAP
            writefile << meld_parallel_time(row, pow(10, -1), pow(2, 8) - 1) << ",";
            writefile << meld_parallel_time(row, pow(10, -1), pow(2, 10) - 1) << ",";
            writefile << meld_parallel_time(row, pow(10, -1), 5, true) << ","; // dynamic chunk_size
            writefile << insert_all_time(row, pow(10, -1)) << ",";
            std::cout << "1. Build finished \n";
            // NUMBER OF COMPARISONS, SORTEDNESS
            // Comparisons using SSH with eps = 0.1
            {
                soft_sequence_heap<int> *softHeap;
                num_comparisons = 0;
                softHeap = insert_meld_ssh(row, pow(10, -1), pow(2, 10) - 1);
                std::vector<int> *extracted = extract_all(softHeap);
                writefile << num_comparisons << ",";
                // Sortedness
                writefile << inversion_count::inv_count(*extracted, j) << ",";
                writefile << rem(*extracted, j) << ",";
                writefile << runs(*extracted, j) << ",";
                writefile << rank_diff(extracted) << ",";
                writefile << exc(*extracted, j) << ",";
                delete softHeap;
                delete extracted;
            }
            // Comparisons using SSH with eps = 0.01
            {
                soft_sequence_heap<int> *softHeap;
                num_comparisons = 0;
                softHeap = insert_meld_ssh(row, pow(100, -1), pow(2, 10) - 1);
                std::vector<int> *extracted = extract_all(softHeap);
                writefile << num_comparisons << ",";
                // Sortedness
                writefile << inversion_count::inv_count(*extracted, j) << ",";
                writefile << rem(*extracted, j) << ",";
                writefile << runs(*extracted, j) << ",";
                writefile << rank_diff(extracted) << ",";
                writefile << exc(*extracted, j) << ",";
                delete softHeap;
                delete extracted;
            }
            // Comparisons using SSH with eps = 0.001
            {
                soft_sequence_heap<int> *softHeap;
                num_comparisons = 0;
                softHeap = insert_meld_ssh(row, pow(1000, -1), pow(2, 10) - 1);
                std::vector<int> *extracted = extract_all(softHeap);
                writefile << num_comparisons << ",";
                // Sortedness
                writefile << inversion_count::inv_count(*extracted, j) << ",";
                writefile << rem(*extracted, j) << ",";
                writefile << runs(*extracted, j) << ",";
                writefile << rank_diff(extracted) << ",";
                writefile << exc(*extracted, j) << ",";
                delete softHeap;
                delete extracted;
            }
            std::cout << "2. Soft Sequence Heaps and Sortedness finished \n";
            // Sorting by Witnesses - Number of Comparisons
            num_comparisons = 0;
            num_comparisons_interval_sorting = 0;
            writefile << insert_extract_interval_sorting_time(row, pow(100, -1) , false, 0, true) << ","; // eps = 0.01
            writefile << num_comparisons_interval_sorting << ",";
            num_comparisons = 0;
            num_comparisons_interval_sorting = 0;
            writefile << insert_extract_interval_sorting_time(row, pow(1000, -1), false, 0, true) << ",";
            writefile << num_comparisons_interval_sorting << ",";
            num_comparisons = 0;
            num_comparisons_interval_sorting = 0;
            writefile << insert_extract_interval_sorting_time(row, pow(10000, -1), false, 0, true) << ",";
            writefile << num_comparisons_interval_sorting << ",";
            std::cout << "3. Interval Sorting finished \n";

            // Sequence Heapsort (without corruption)
            num_comparisons = 0;
            writefile << insert_extract_all_time(row, 2*pow(j,-1)) << ",";
            writefile << num_comparisons << ",";
            std::cout << "4. Sequence Heap finished \n";

            // SORTING ALGORITHMS
            num_comp_quicksort = 0;
            writefile << quicksort_time(row, j) << ",";
            writefile << num_comp_quicksort << ",";

            num_comp_merge_sort = 0;
            writefile << mergesort_time(row, j) << ",";
            writefile << num_comp_merge_sort << ",";

            num_comparison_timsort = 0;
            writefile << timsort_time(row, j) << ",";
            writefile << num_comparison_timsort << ",";

            num_comp_binary_heapsort = 0;
            writefile << binary_heap_time(row) << ",";
            writefile << num_comp_binary_heapsort << ",";

            std::vector<int> row_cpy(row);
            writefile << std_sort_time(row_cpy) << ",";
            std::cout << "5. Sorting Algorithms finished \n";

            // COMBINED SORTING
            num_comparisons = 0;
            num_comp_merge_sort = 0;
            writefile << combine_ssh_mergesort(row, j, pow(1000, -1)) << ",";
            writefile << num_comparisons + num_comp_merge_sort << ",";
            num_comparisons = 0;
            num_comparison_timsort = 0;
            writefile << combine_ssh_timsort(row, j, pow(1000, -1)) << ",";
            writefile << num_comparisons + num_comparison_timsort << ",";
            std::cout << "6. Combined Sorting finished \n";

            // NEAR SORTING APPROACH CHAZELLE
//            num_comparisons = 0;
//            num_comparison_timsort = 0;
//            num_comparisons_chazelle_sorting = 0;
            writefile << chazelle_near_sorting_time(row, j, pow(1000, -1));
            // to measure the number of comparisons use the function find_in_list in line 72-73 (chazelle_sorting.h)
            // instead of std::find in line 68-69
//            writefile << num_comparisons_chazelle_sorting;
            std::cout << "7. Chazelle Sorting finished \n \n";
            writefile << "\n";
            writefile.flush();
        }
    }
    writefile.close();
    myfile.close();
}


