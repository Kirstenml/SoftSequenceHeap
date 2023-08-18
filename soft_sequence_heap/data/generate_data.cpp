//
// Generate Data for testing and analyzing all presented (in my Master's Thesis) sorting algorithms
//

#include "generate_data.h"

// generate a partly sorted sequence where prob denotes the probability (in percentage) which areas are permuted
void sort_partially(std::vector<int> &a, const int n, const int prob) {
    int j;
    srand((unsigned) time(0));
    for (int i = 0; i < n; ++i) {
        if ((rand()%100) <= prob) {
            // choose area to permute
            j = rand()%(n-i);
            if (j == 0) { continue; }
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(a.begin() + i, a.begin() + i + j - 1, g);
            i += j - 1;
        }
    }
}


// generate random permutations of finite sequences
int *fisher_yates_shuffle(const int *arr, const int n) {
    int *a = new int[n];
    int *ind = new int[n];
    srand((unsigned) time(0));
    for (int i = 0; i < n; ++i)
        ind[i] = 0;
    int index;
    for (int i = 0; i < n; ++i) {
        do {
            index = rand() % n;
        }  while (ind[index] != 0);
        ind[index] = 1;
        a[i] = *(arr + index);
    }
    return a;
}


// write permutations of length n (in array len) to a file (numbers from 0 to n-1)
// disorder = 1: partially sort the sequence with probability prob
// disorder = 2: apply std::shuffle because Fisher-Yates-Shuffle takes too long
void write_permutations(const int *len, const int num, const int disorder, bool large, const int prob) {
    std::ofstream myfile;
    if (disorder == 1) {
        if (!large) { myfile.open("../data/partially_sorted.txt"); }
        else { myfile.open("../data/partially_sorted_large.txt"); }
    } else if (disorder == 2){
        if (!large) { myfile.open("../data/random_permutations.txt"); }
        else { myfile.open("../data/random_permutations_large.txt"); }
    }
    int n;
    for (int j = 0; j < num; ++j) {
        n = len[j];
        std::cout << "j=" << n << ", ";
        std::vector<int> vec;
        for (int i = 0; i < n; ++i) {
            vec.push_back(i);
        }
        if (disorder == 1) {
            sort_partially(vec, n, prob);
        } else if (disorder == 2) {
//            a = fisher_yates_shuffle(&vec[0], n);
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(vec.begin(), vec.end(), g);
        }

        // write permutation to file
        for (int i = 0; i < n-1; ++i) {
            myfile << vec[i] << ",";
        }
        myfile << vec[n-1];
        myfile << "\n";
    }
    myfile.close();
}

void generate_data(int order_permutations, bool large, int prob) {
    int num = 23;
    std::vector<int> len;
    if (! large) {
        len.push_back(100);
        len.push_back(1000);
        for (int i = 2; i < num; ++i) {
            len.push_back(len.back() + 1000);
        }
    } else {
        // we generate data up to sizes of 10^8
        len.push_back(100);
        len.push_back(5000);
        len.push_back(10000);   // 10^4
        for (int i = 1; i <= num-3; ++i) {
            len.push_back(i*5000000);    // append i * 5 * 10^6
        }
    }
    len.shrink_to_fit();
    if (order_permutations != 0) { write_permutations(&len[0], num, order_permutations, large, prob); }
}
