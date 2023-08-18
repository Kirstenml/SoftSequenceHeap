//
// Soft Sequence Heaps (an alternative implementation of Chazelle's soft heaps) introduced 2021 by Brodal
//

#ifndef SOFTSEQUENCEHEAP_SOFT_SEQUENCE_HEAP_TPP
#define SOFTSEQUENCEHEAP_SOFT_SEQUENCE_HEAP_TPP


template<typename T>
soft_sequence_heap<T>::soft_sequence_heap() {}

template<typename T>
soft_sequence_heap<T> *make_heap(const float& eps){
    soft_sequence_heap<T> *s = new soft_sequence_heap<T>();
    if (eps < 0 or eps > 1) {
        throw "Invalid input";
    }
    s->rank_threshold = (int64_t) ceil(std::log2(pow(eps, -1)));
    s->head_list = nullptr;
    s->lazily_deleted = std::list<T>();
    return s;
}

// insert_head_at_front h before current head head_list_c
template<typename T>
void insert_head_at_front(head<T> *head_list_c, head<T> *h) {
    if (head_list_c == nullptr) {
        head_list_c = h;
        return;
    }
    // insert_head_at_front h at the beginning of head-list
    if (head_list_c->prev == nullptr) {
        head_list_c->prev = h;
        h->next = head_list_c;
    }
}

template<typename T>
head<T>* merge_heads(head<T> *h1, head<T> *h2) {
    if (h1->rank != h2->rank) {
        throw "Invalid rank size detected while merging two heads.";
    }
    auto *h_merged = new head<T>();
    h_merged->rank = h1->rank + 1;
    binary_merge(h1, h2, h_merged);
    return h_merged;
}

// Update suffix min pointer of all heads with rank smaller or equal to rank of head h
template<typename T>
head<T> *update_suffix_min(head<T> *h) {
    head<T> *tmp_min;
    if (h->next == nullptr) {
        tmp_min = h;
    } else {
        tmp_min = h->next->suffix_min;
    }
    // walk backwards through head-list
    while (true) {
        if (h->sequence.front().key < tmp_min->sequence.front().key) { tmp_min = h; }
        ++num_comparisons;
        h->suffix_min = tmp_min;
        if (h->prev == nullptr) { break; }
        h = h->prev;
    }
    return h;
}

template<typename T>
void update_suffix_min_once(head<T> *h) {
    if (h->next == nullptr) {
        h->suffix_min = h;
        return;
    }
    if (h->next->suffix_min->sequence.front().key < h->sequence.front().key) {
        h->suffix_min = h->next->suffix_min;
    } else {
        h->suffix_min = h;
    }
    ++num_comparisons;
}

// replace the first two heads of head-list (first_head and snd_head) with replace_head
template<typename T>
head<T>* replace_heads(head<T> *first_head, head<T> *snd_head, head<T> *replace_head) {
    if (first_head->prev == nullptr and snd_head->next == nullptr) {    // head-list is empty
        replace_head->prev = nullptr;
        replace_head->next = nullptr;
    } else if (first_head->prev == nullptr) {
        replace_head->prev = nullptr;
        replace_head->next = snd_head->next;
        snd_head->next->prev = replace_head;
    } else if (snd_head->next == nullptr) {
        replace_head->prev = first_head->prev;
        replace_head->next = nullptr;
        first_head->prev->next = replace_head;
    } else {
        replace_head->prev = first_head->prev;
        replace_head->next = snd_head->next;
        snd_head->next->prev = replace_head;
        first_head->prev->next = replace_head;
    }
    delete first_head;
    delete snd_head;
    return replace_head;
}

template<typename T>
void insert(soft_sequence_heap<T> *s, const T &value) {
    auto *h = new head<T>(node<T>(value));     // head to be inserted
    head<T> *h_merged;
    insert_head_at_front(s->head_list, h);
    s->head_list = h;   // h is now the first head in head-list
    // iterate through head-list
    while (s->head_list->next != nullptr and s->head_list->rank == s->head_list->next->rank) {
        h_merged = merge_heads(s->head_list, s->head_list->next);
//        if (h_merged->rank > s->rank_threshold and (h_merged->rank - s->rank_threshold) % 3 != 0) {
        if (h_merged->rank > s->rank_threshold and (h_merged->rank - s->rank_threshold) % 2 == 0) {
            prune(h_merged->sequence);
        }
        s->head_list = replace_heads(s->head_list, s->head_list->next, h_merged);
    }
    update_suffix_min_once(s->head_list);
}

template<typename T>
void insert(soft_sequence_heap<T> *s, const std::vector<T> &values) {
    for (auto const &v : values) {
        insert(s, v);
    }
}

template<typename T>
tuple_find_min<T>* find_min(soft_sequence_heap<T> *s) {
    if (s->head_list == nullptr) { return nullptr; }
    head<T> *min_head = s->head_list->suffix_min;
    node<T> *min_node = &(min_head->sequence.front());
    if (min_node->corruption_set.empty()) {
        return new tuple_find_min<T>(min_node->key, min_node->key);
    }
    return new tuple_find_min<T>(min_node->corruption_set.front(), min_node->key);
}

// delete lazily be value (more expensive than deleting by pointer)
template<typename T>
circular_linked_list<T> *delete_item(soft_sequence_heap<T> *s, T item) {
    if (empty(s)) { throw "Soft Sequence Heap is empty"; }
    tuple_find_min<T>* findMin = find_min(s);
    T min_node = findMin->real_key;
    if (min_node != item) {
        s->lazily_deleted.push_back(item);
        return nullptr;
    }
    delete findMin;
    return extract_min(s).corruption_set;
}

template<typename T>
head<T>* remove_head(soft_sequence_heap<T> *s, head<T> *h) {
    if (h == nullptr) { return nullptr; }
    head<T> *prev_head = h->prev;
    if (h->prev == nullptr and h->next == nullptr) {
        s->head_list = nullptr;
    } else if (h->prev == nullptr) {
        h->next->prev = nullptr;
        s->head_list = h->next;
    } else if (h->next == nullptr) {
        h->prev->next = nullptr;
    } else {
        h->prev->next = h->next;
        h->next->prev = h->prev;
    }
    delete h;
    return prev_head;
}

// removes all items in the intersection of corrupted_items and lazily_deleted from corrupted_items
template<typename T>
void remove_lazily_deleted(soft_sequence_heap<T> *s, std::list<T> &corrupted_items) {
    if (corrupted_items.empty()) { return; }
    if (s->lazily_deleted.empty()) { return; }

    bool found;
    for (auto it = corrupted_items.begin(); it != corrupted_items.end(); ) {
        found = (std::find(s->lazily_deleted.begin(), s->lazily_deleted.end(), *it) != s->lazily_deleted.end());
        if (found) {
            it = corrupted_items.erase(it);
        } else { ++it; }
    }
}

// delete a node with minimum current_key and eventually report a set of items as corrupted
template<typename T>
triple_extract_min<T> extract_min(soft_sequence_heap<T> *s) {
    if (s->head_list == nullptr) {
        throw "Soft Sequence Heap is empty";
    }
    head<T> *min_head = s->head_list->suffix_min;
    node<T> *min_node = &(min_head->sequence.front());

    if (! (min_node->corruption_set.empty())) {    // corruption set is not empty
        T min_val = min_node->corruption_set.pop_front();
        T min_node_key = min_node->key;
        // check if new minimum has to be deleted
        while ((! (min_node->corruption_set.empty())) and
               std::find(s->lazily_deleted.begin(), s->lazily_deleted.end(),
                         min_node->corruption_set.front()) != s->lazily_deleted.end()) {
            min_node->corruption_set.pop_front();
        } // if C is now empty and min_node has to be deleted
        if (min_node->corruption_set.empty() and
            (std::find(s->lazily_deleted.begin(), s->lazily_deleted.end(), min_node->key)
             != s->lazily_deleted.end())) {
            extract_min(s);
        }
        return triple_extract_min<T>(min_val, min_node_key, std::list<T>());
    }
    // Corrupt all items in witness set
    std::list<T> corrupted_items;
    min_node->witness_set.to_list(corrupted_items);
    T min_val = min_node->key;
    // delete minimum node
    min_head->sequence.pop_front();
    // remove all items from corrupted_items that are lazily deleted
    remove_lazily_deleted(s, corrupted_items);
    // min_head becomes empty after deleting the first node
    if (min_head->sequence.empty()) {
        head<T> *prev_head = remove_head(s, min_head);
        if (prev_head != nullptr) { update_suffix_min(prev_head); }
    } else {
        update_suffix_min(min_head);    // head has a new root
    }
    if (s->head_list == nullptr) {
        return triple_extract_min<T>(min_val, min_val, corrupted_items);
    }
    // if the new minimum item is marked as lazily deleted
    // repeatedly remove this until new minimum is not lazily deleted or heap is empty
    tuple_find_min<T>* findMin = find_min(s);
    tuple_find_min<T>* findMin_tmp;
    T n_min_val = findMin->real_key;
    // while new minimum value (n_min_val) has to be lazily deleted, i.e. is element of lazily_deleted
    while ((std::find(s->lazily_deleted.begin(), s->lazily_deleted.end(), n_min_val) != s->lazily_deleted.end())) {
        // delete n_min_val and accumulate all generated corruptions in corrupted_items
        std::list<T> l = extract_min(s).corruption_set;
        corrupted_items.splice(corrupted_items.end(), l);
        findMin_tmp = find_min(s);
        delete findMin;
        findMin = findMin_tmp;
        n_min_val = findMin->real_key;
    }
    delete findMin;
    return triple_extract_min<T>(min_val, min_val, corrupted_items);
}


// call repeatedly on all items in the soft heap extract-min and return an vector sorted by keys
template<typename T>
std::vector<T> *extract_all(soft_sequence_heap<T> *s) {
    std::vector<T> *extracted = new std::vector<T>();
    while (!empty(s)) {
        {
            triple_extract_min<T> t_extract = extract_min(s);
            extracted->push_back(t_extract.real_key);    // expensive!
        }
    }
    return extracted;
}

// call repeatedly on all items in the soft heap extract-min and return nothing (only for experiments)
template<typename T>
void extract_all_no_return(soft_sequence_heap<T> *s) {
    while (!empty(s)) {
        {
            extract_min(s);
        }
    }
}

template<typename T>
bool empty(soft_sequence_heap<T> *s) {
    return s->head_list == nullptr;
}

// append h1 to h
template<typename T>
head<T> *append_head(head<T> *h, head<T> *h1) {
    if (h == nullptr) {
        h = h1;
        return h;
    }
    h->next = h1;
    h1->prev = h;
    return h1;
}

template<typename T>
soft_sequence_heap<T>* meld(soft_sequence_heap<T> *s1, soft_sequence_heap<T> *s2) {
    soft_sequence_heap<T> *s = make_heap<T>(0.5);
    s->rank_threshold = s1->rank_threshold;
    // combine lazily deleted lists from s1 and s2
    s->lazily_deleted = std::list<T>(s1->lazily_deleted.begin(), s1->lazily_deleted.end());
    s->lazily_deleted.insert(s->lazily_deleted.end(), s2->lazily_deleted.begin(), s2->lazily_deleted.end());
    // meld head lists together
    head<T> *c_h1 = s1->head_list;
    head<T> *c_h2 = s2->head_list;
    if (c_h1 == nullptr) { return s2; }
    if (c_h2 == nullptr) { return s1; }
    head<T> *c_h = s->head_list;
    while (c_h1 != nullptr and c_h2 != nullptr) {
        if (c_h1->rank < c_h2->rank) {
            c_h = append_head(c_h, c_h1);
            c_h1 = c_h1->next;
        } else {
            c_h = append_head(c_h, c_h2);
            c_h2 = c_h2->next;
        }
    }
    if (c_h1 == nullptr) {      // append rest of h2 to h
        c_h->next = c_h2;
        c_h2->prev = c_h;
    } else if(c_h2 == nullptr) {    // append rest of h1 to h
        c_h->next = c_h1;
        c_h1->prev = c_h;
    }
    s1->head_list = nullptr;
    s2->head_list = nullptr;
    delete s1;
    delete s2;

    // possible that a head of same rank is appended
    // but we do not need to go further bc c_h1 or c_h2 are soft sequences with heads of distinct rank
    if (c_h->next != nullptr) { c_h = c_h->next; }
    head<T> *h;
    // while two sequences in s have equal rank, merge them and apply eventually prune
    while (c_h->prev != nullptr) {
        if(c_h->rank == c_h->prev->rank) {
            c_h = c_h->prev;
            // iterate through head-list
            while (c_h->next != nullptr and c_h->rank == c_h->next->rank) {
                h = merge_heads(c_h, c_h->next);
                if (h->rank > s->rank_threshold and (h->rank - s->rank_threshold) % 2 == 0) {
                    prune(h->sequence);
                }
                c_h = replace_heads(c_h, c_h->next, h);
            }
        }
        update_suffix_min_once(c_h);
        if (c_h->prev == nullptr) { break; }
        c_h = c_h->prev;
    }
    update_suffix_min_once(c_h);
    s->head_list = c_h;

    // delete soft heaps
//    s1->head_list = nullptr;
//    s2->head_list = nullptr;


    return s;
}

/////// Sorting by Witnesses - modified extract-min method

// delete a node with minimum current_key and eventually report a set of items as corrupted
template<typename T>
triple_extract_min_sorting<T> extract_min_sbw(soft_sequence_heap<T> *s) {
    if (s->head_list == nullptr) {
        throw "Soft Sequence Heap is empty";
    }
    head<T> *min_head = s->head_list->suffix_min;
    node<T> *min_node = &(min_head->sequence.front());

    if (! (min_node->corruption_set.empty())) {    // corruption set is not empty
        size_t corruption_set_size = min_node->corruption_set.size();
        T min_val = min_node->corruption_set.pop_front();
        T min_node_key = min_node->key;
        // check if new minimum has to be deleted
        while ((! (min_node->corruption_set.empty())) and
               std::find(s->lazily_deleted.begin(), s->lazily_deleted.end(),
                         min_node->corruption_set.front()) != s->lazily_deleted.end()) {
            min_node->corruption_set.pop_front();
        } // if C is now empty and min_node has to be deleted
        if (min_node->corruption_set.empty() and
            (std::find(s->lazily_deleted.begin(), s->lazily_deleted.end(), min_node->key)
             != s->lazily_deleted.end())) {
            extract_min(s);
        }
        return triple_extract_min_sorting<T>(min_val, corruption_set_size, std::list<T>());
    }
    // Corrupt all items in witness set
    std::list<T> corrupted_items;
    min_node->witness_set.to_list(corrupted_items);
    T min_val = min_node->key;
    // delete minimum node
    min_head->sequence.pop_front();
    // remove all items from corrupted_items that are lazily deleted
    remove_lazily_deleted(s, corrupted_items);
    // min_head becomes empty after deleting the first node
    if (min_head->sequence.empty()) {
        head<T> *prev_head = remove_head(s, min_head);
        if (prev_head != nullptr) { update_suffix_min(prev_head); }
    } else {
        update_suffix_min(min_head);    // head has a new root
    }
    if (s->head_list == nullptr) {
        return triple_extract_min_sorting<T>(min_val, min_val, corrupted_items);
    }
    // if the new minimum item is marked as lazily deleted
    // repeatedly remove this until new minimum is not lazily deleted or heap is empty
    tuple_find_min<T>* findMin = find_min(s);
    tuple_find_min<T>* findMin_tmp;
    T n_min_val = findMin->real_key;
    // while new minimum value (n_min_val) has to be lazily deleted, i.e. is element of lazily_deleted
    while ((std::find(s->lazily_deleted.begin(), s->lazily_deleted.end(), n_min_val) != s->lazily_deleted.end())) {
        // delete n_min_val and accumulate all generated corruptions in corrupted_items
        std::list<T> l = extract_min(s).corruption_set;
        corrupted_items.splice(corrupted_items.end(), l);
        findMin_tmp = find_min(s);
        delete findMin;
        findMin = findMin_tmp;
        n_min_val = findMin->real_key;
    }
    delete findMin;
    return triple_extract_min_sorting<T>(min_val, 0, corrupted_items);
}



/////// PRINTING
template<typename T>
std::ostream& operator<<(std::ostream &out, soft_sequence_heap<T> const& softHeap) {
    out << "rank threshold = " << softHeap.rank_threshold << std::endl;
    head<T> *h_c = softHeap.head_list;
    while (h_c != nullptr) {
        out << *h_c;
        h_c = h_c->next;
    }
    return out;
}

template<typename T>
std::ostream& operator<<(std::ostream &out, triple_extract_min<T> const& tripleExtractMin) {
    if (tripleExtractMin.corruption_set->empty()) { return out; }
    out << "minimum value (maybe corrupted) = " << tripleExtractMin.real_key << std::endl;
    out << "minimum current_key = " << tripleExtractMin.current_key << std::endl;
    out << "corrupted items = " << *(tripleExtractMin.corruption_set) << std::endl;
    return out;
}

#endif //SOFTSEQUENCEHEAP_SOFT_SEQUENCE_HEAP_TPP
