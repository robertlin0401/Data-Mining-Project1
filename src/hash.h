#ifndef HASH_H
#define HASH_H

#include <stdbool.h>
#include "itemset.h"

typedef struct hash {
    void *ptr[10]; // Hash * or Itemset *
    int length[10];
    bool is_hash[10];
} Hash;

/* Allocate a new hash table and initialize its content. */
Hash *new_hash();

/* Split itemset list that reaches the maximum size, 
 * and distribute each itemset to the hash table.
 */
void split_itemset_to_hash(Itemset *target, Hash *hash_ptr, int itemID_digit, int level);

/* Insert a new itemset into the target candidate hash table. */
void insert_itemset_into_hash(Hash *target, Itemset *itemset_ptr, int itemID_digit, int level);

#endif
