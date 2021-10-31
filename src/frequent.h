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

void generate_candidate(int itemID_digit)
{
    if (!candidate_head) { // level-1 candidates
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

void count_support_of_candidate(int itemID_digit)
{
    /* Traverse through the transactions and count the appearing time of each candidate itemset. */
    transaction_ptr = transaction_head;
    while (transaction_ptr) {
        Itemset *transaction_itemset = new_itemset();
        transaction_itemset->item_list_head = transaction_ptr->item_list_head;
        transaction_itemset->length = transaction_ptr->length;

        while (transaction_itemset->item_list_head) {
            Itemset *candidate_itemset = look_up_hash(candidate_head, transaction_itemset, itemID_digit, 1);
            while (candidate_itemset) {
                if (is_sub_itemset(candidate_itemset, transaction_itemset))
                    candidate_itemset->count++;
                candidate_itemset = candidate_itemset->next;
            }
            transaction_itemset->item_list_head = transaction_itemset->item_list_head->next;
        }
        
        if (transaction_ptr->next)
            transaction_ptr = transaction_ptr->next;
        else
            break;
    }
    print_hash(candidate_head);
}

#endif
