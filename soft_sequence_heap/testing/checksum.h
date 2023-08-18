//
// Checksum calculations for benchmarking
//

#ifndef SOFTSEQUENCEHEAP_CHECKSUM_H
#define SOFTSEQUENCEHEAP_CHECKSUM_H

#include <vector>
#include "../soft_sequence_heap.h"
#include <math.h>       /* ceil */

unsigned long long highest_rank_checksum(soft_sequence_heap<int> *s);
int extract_all_checksum(soft_sequence_heap<int> *s);

#endif //SOFTSEQUENCEHEAP_CHECKSUM_H
