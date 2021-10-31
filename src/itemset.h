#ifndef ITEMSET_H
#define ITEMSET_H

#include <stdbool.h>
#include "transaction.h"

typedef struct itemset {
    Item *item_list_head;
    int count;
    int length;
    struct itemset *next;
} Itemset;

/* Allocate a new itemset and initialize its content. */
Itemset *new_itemset();

/* Allocate a new item, insert it into the target itemset at
 * the position where it should be.
 */
void insert_item_into_itemset(Itemset *target, int itemID);

/* Generate all subsets with specific length from target itemset,
 * then return the pointer to the list of subsets.
 */
Itemset *generate_sub_itemset(Itemset *itemset_ptr, int length);

/* Check if target itemset is the subset of comparison itemset. */
bool is_sub_itemset(Itemset *target, Itemset *comparison);

/* Check if target itemset is identical to comparison itemset. */
bool is_identical_itemset(Itemset *target, Itemset *comparison);

/* Print out the item list of target itemset. */
void print_itemset(Itemset *target);

#endif
