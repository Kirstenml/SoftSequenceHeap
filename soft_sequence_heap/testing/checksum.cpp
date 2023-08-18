//
// Checksum calculations for benchmarking
//

#include "checksum.h"

unsigned long long highest_rank_checksum(soft_sequence_heap<int> *s) {
    unsigned long long checksum = 0;
    head<int> *current_head = s->head_list;
    while (current_head->next != nullptr) {
        ++checksum;
        current_head = current_head->next;
    }
    return checksum;
}

int extract_all_checksum(soft_sequence_heap<int> *s) {
    int checksum = 0;
    while (!empty(s)) {   // corruption_set is a nullptr if soft sequence heap is empty
        {
            triple_extract_min<int> t_extract = extract_min(s);
            checksum += t_extract.real_key;
        }
    }
    return checksum;
}
