#include <iostream>
#include "soft_sequence_heap.h"
#include <vector>
#include "data/generate_data.h"
#include "data/read_data.h"
#include "benchmarks.h"

int main() {
/* order_permutations = 0: sorted
 *                    = 1: partially sorted
 *                    = 2: random
*/
    int order_permutations = 2;
    bool large = true;

    /////// GENERATE DATA
//    generate_data(order_permutations, large, 30);
//    return 0;

    /////// TIME MEASUREMENTS FOR BUILDING A SOFT SEQUENCE HEAP WITH LARGE DATA

    write_benchmarks(2);
    write_benchmarks(1);


    /////// TEST SOFT SEQUENCE HEAP
//    float eps = 0.8;
//    soft_sequence_heap<int> *softHeap = make_heap<int>(eps);
//    soft_sequence_heap<int> *softHeap2 = make_heap<int>(eps);
//
//    std::vector<int> numbers{13, 5, 30, 20, 50, 14, 26, 33, 21, 4, 17, 44, 23, 9, 11};
//    std::vector<int> numbers2{1, 2, 6, 7, 23, 7, 18, 16, 70, 66, 3};
//    insert(softHeap, numbers);
//    insert(softHeap2, numbers2);
//
//    std::cout << *softHeap << std::endl;
//    std::cout << *softHeap2 << std::endl;
//
//    /////// meld
//    soft_sequence_heap<int> *s = meld(softHeap, softHeap2);
//    std::cout << *s << std::endl;
//    extract_all(s);

    /////// extract_min
//    std::cout << extract_min(s) << std::endl;
//    std::cout << extract_min(s) << std::endl;
//    std::cout << extract_min(s) << std::endl;


    /////// delete
//    delete_item(s, 26);
//    std::cout << *s << std::endl;
//
//    int n = numbers.size() + numbers2.size();
//    std::vector<int> result(n);
//    for (int i = 0; i < n; ++i) {
//        triple_extract_min<int> extract_s = extract_min(s);
//        std::cout << extract_s << std::endl;
//        result[i] = extract_s.real_key;
//    }
//    for (int i = 0; i < n; ++i) {
//        std::cout << result[i] << ", ";
//    }

    /////// EXAMPLE FROM MASTER'S THESIS
//    soft_sequence_heap<int> *softHeap_example = make_heap<int>(eps);
//    insert(softHeap_example, numbers);
//
//    extract_min(softHeap_example);
//    extract_min(softHeap_example);
//    std::cout << std::endl;
//    std::cout << *softHeap_example << std::endl;
//    std::cout << "insert 19: " << std::endl;
//    std::cout << std::endl;
//    insert(softHeap_example, 19);
//    std::cout << *softHeap_example << std::endl;
//    std::vector<int> *res = extract_all(softHeap_example);
//    for (int val : *res) {
//        std::cout << val << ", ";
//    }
//    delete res;
    return 0;
}
