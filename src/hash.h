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

/* Look up the hash table, find the position where the itemset
 * belongs to, and return the pointer to that position.
 */
Itemset *look_up_hash(Hash *target, Itemset *itemset_ptr, int itemID_digit, int level);

/* Split itemset list that reaches the maximum size, 
 * and distribute each itemset to the hash table.
 */
void split_itemset_to_hash(Itemset *target, Hash *hash_ptr, int itemID_digit, int level);

/* Insert a new itemset into the target candidate hash table. */
void insert_itemset_into_hash(Hash *target, Itemset *itemset_ptr, int itemID_digit, int level);

/* Traverse the hash table in DFS order, then return the pointer
 * to the result of itemset list .
 */
Itemset *DFS_traversal(Hash *target);

/* Free the hash table and its content, except the itemset list inside. */
void free_hash(Hash *target);

/* Print out the content of target hash table in DFS order. */
void print_hash(Hash *target);

#endif
