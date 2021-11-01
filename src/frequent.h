#ifndef FREQUENT_H
#define FREQUENT_H

#include <stdbool.h>
#include "transaction.h"
#include "itemset.h"
#include "hash.h"

typedef struct frequent {
    Itemset *itemset_list_head;
    int level;
    struct frequent *next;
} Frequent;

Frequent *frequent_head = NULL;
Frequent *frequent_ptr = NULL;

Hash *candidate_head = NULL;

int frequent_level = 0;

void print_frequent_itemset(Frequent *target)
{
    Itemset *itemset_list_ptr = target->itemset_list_head;
    while (itemset_list_ptr) {
        print_itemset(itemset_list_ptr);
        printf("\n");
        itemset_list_ptr = itemset_list_ptr->next;
    }
}

void generate_candidate(int itemID_digit)
{
    if (frequent_level == 1) { // level-1 candidates
        candidate_head = new_hash();
        Item *item_list_head = NULL;

        /* Traverse through the transactions and find all unique items. */
        transaction_ptr = transaction_head;
        while (transaction_ptr) {
            transaction_ptr->item_list_ptr = transaction_ptr->item_list_head;
            while (transaction_ptr->item_list_ptr) {

                /* Build an item list in increasing order. */
                if (!item_list_head) {
                    item_list_head = new_item(transaction_ptr->item_list_ptr->itemID);
                } else {
                    if (item_list_head->itemID > transaction_ptr->item_list_ptr->itemID) {
                        Item *item_ptr = new_item(transaction_ptr->item_list_ptr->itemID);
                        item_ptr->next = item_list_head;
                        item_list_head = item_ptr;
                    } else {
                        Item *item_list_ptr = item_list_head;
                        bool is_unique = 1;
                        while (item_list_ptr->next) {
                            if (item_list_ptr->itemID == transaction_ptr->item_list_ptr->itemID) {
                                is_unique = 0;
                                break;
                            }
                            if (item_list_ptr->next->itemID > transaction_ptr->item_list_ptr->itemID)
                                break;
                            item_list_ptr = item_list_ptr->next;
                        }
                        if (item_list_ptr->itemID == transaction_ptr->item_list_ptr->itemID)
                            is_unique = 0;
                        if (is_unique) {
                            Item *item_ptr = new_item(transaction_ptr->item_list_ptr->itemID);
                            item_ptr->next = item_list_ptr->next;
                            item_list_ptr->next = item_ptr;
                        }
                    }
                }

                if (transaction_ptr->item_list_ptr->next)
                    transaction_ptr->item_list_ptr = transaction_ptr->item_list_ptr->next;
                else
                    break;
            }
            if (transaction_ptr->next)
                transaction_ptr = transaction_ptr->next;
            else
                break;
        }

        /* Build the hash table based on the item list bulit in last step. */
        while (item_list_head) {
            Item *temp = item_list_head;
            item_list_head = item_list_head->next;
            Itemset *itemset_ptr = new_itemset();
            insert_item_into_itemset(itemset_ptr, temp->itemID);
            insert_itemset_into_hash(candidate_head, itemset_ptr, itemID_digit, 1);
            free(temp);
        }
    } else {

    }
    // print_hash(candidate_head);
}

/* Traverse through the transactions and generate sub-itemsets for each transaction.
 * Look up each sub-itemset in the hash table and increase the count of the found
 * candidate.
 */
void count_support_of_candidate(int itemID_digit)
{
    transaction_ptr = transaction_head;
    while (transaction_ptr) {
        Itemset *transaction_itemset = new_itemset();
        transaction_itemset->item_list_head = transaction_ptr->item_list_head;
        transaction_itemset->length = transaction_ptr->length;
        Itemset *transaction_sub_itemset = generate_sub_itemset(transaction_itemset, frequent_level);

        Itemset *transaction_sub_ptr = transaction_sub_itemset;
        while (transaction_sub_ptr) {
            Itemset *candidate_itemset = look_up_hash(candidate_head, transaction_sub_ptr, itemID_digit, 1);
            while (candidate_itemset) {
                if (is_identical_itemset(candidate_itemset, transaction_sub_ptr))
                    candidate_itemset->count++;
                candidate_itemset = candidate_itemset->next;
            }
            transaction_sub_ptr = transaction_sub_ptr->next;
        }
        
        free_itemset_list(transaction_sub_itemset);

        if (transaction_ptr->next)
            transaction_ptr = transaction_ptr->next;
        else
            break;
    }
    // print_hash(candidate_head);
}

/* Increase the frequent level and allocate a new frequent itemset data structure. */
void new_frequent_level()
{
    Frequent *new_frequent = (Frequent *)malloc(sizeof(Frequent));
    new_frequent->itemset_list_head = NULL;
    new_frequent->level = ++frequent_level;
    new_frequent->next = NULL;
    if (!frequent_ptr)
        frequent_head = frequent_ptr = new_frequent;
    else {
        frequent_ptr->next = new_frequent;
        frequent_ptr = new_frequent;
    }
}

/* Traverse through the candidate hash table and pick candidates with support value
 * greater than minimum requirement, insert them into the frequent itemset.
 */
void generate_frequent_itemset(int support_count)
{
    Itemset *itemset_list_ptr = DFS_traversal(candidate_head);
    Itemset *frequent_list_ptr = NULL, *infrequent_list_ptr = NULL;
    while (itemset_list_ptr) {
        Itemset *temp = itemset_list_ptr;
        itemset_list_ptr = itemset_list_ptr->next;
        temp->next = NULL;
        if (temp->count < support_count)
            infrequent_list_ptr = insert_itemset_into_list(temp, infrequent_list_ptr);
        else
            frequent_list_ptr = insert_itemset_into_list(temp, frequent_list_ptr);
    }
    frequent_ptr->itemset_list_head = frequent_list_ptr;
    free_itemset_list(infrequent_list_ptr);
    free_hash(candidate_head);
    // print_frequent_itemset(frequent_ptr);
}

#endif
