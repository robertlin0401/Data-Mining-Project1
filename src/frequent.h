#ifndef FREQUENT_H
#define FREQUENT_H

#include "itemset.h"
#include "hash.h"

typedef struct frequent {
    Itemset *itemset_list_head;
    int level;
    struct frequent *next;
} Frequent;

extern Frequent *frequent_head, *frequent_ptr;

extern Hash *candidate_head;

extern int frequent_level;

/* Print out the itemset list in the target frequent level. */
void print_frequent_itemset(Frequent *target);

/* Generate the candidate hash table based on the frequent level. */
void generate_candidate(int itemID_digit);

/* Traverse through the transactions and generate sub-itemsets for each transaction.
 * Look up each sub-itemset in the hash table and increase the count of the found
 * candidate.
 */
void count_support_of_candidate(int itemID_digit);

/* Increase the frequent level and allocate a new frequent itemset data structure. */
void new_frequent_level();

/* Traverse through the candidate hash table and pick candidates with support value
 * greater than minimum requirement, insert them into the frequent itemset.
 */
void generate_frequent_itemset(int support_count);

#endif
