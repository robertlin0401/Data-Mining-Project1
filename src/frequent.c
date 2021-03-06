#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "transaction.h"
#include "frequent.h"

Frequent *frequent_head = NULL;
Frequent *frequent_ptr = NULL;

Hash *candidate_head = NULL;

int frequent_level = 0;

void generate_candidate(int itemID_digit)
{
    candidate_head = new_hash();
    if (frequent_level == 1) { // level-1 candidates
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
                        Item *temp = new_item(transaction_ptr->item_list_ptr->itemID);
                        temp->next = item_list_head;
                        item_list_head = temp;
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
                            Item *temp = new_item(transaction_ptr->item_list_ptr->itemID);
                            temp->next = item_list_ptr->next;
                            item_list_ptr->next = temp;
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
        Itemset *itemset_list_ptr = frequent_ptr->itemset_list_head;
        frequent_ptr = frequent_ptr->next;

        Itemset *itemset_ptr = new_itemset();

        /* Traverse through the itemset list and find all unique items. */
        while (itemset_list_ptr) {
            Item *item_ptr = itemset_list_ptr->item_list_head;
            while (item_ptr) {

                /* Build an item list in increasing order. */
                if (!itemset_ptr->item_list_head) {
                    insert_item_into_itemset(itemset_ptr, item_ptr->itemID);
                } else {
                    if (itemset_ptr->item_list_head->itemID > item_ptr->itemID) {
                        insert_item_into_itemset(itemset_ptr, item_ptr->itemID);
                    } else {
                        Item *item_list_ptr = itemset_ptr->item_list_head;
                        bool is_unique = 1;
                        while (item_list_ptr->next) {
                            if (item_list_ptr->itemID == item_ptr->itemID) {
                                is_unique = 0;
                                break;
                            }
                            if (item_list_ptr->next->itemID > item_ptr->itemID)
                                break;
                            item_list_ptr = item_list_ptr->next;
                        }
                        if (item_list_ptr->itemID == item_ptr->itemID)
                            is_unique = 0;
                        if (is_unique)
                            insert_item_into_itemset(itemset_ptr, item_ptr->itemID);
                    }
                }

                item_ptr = item_ptr->next;
            }
            itemset_list_ptr = itemset_list_ptr->next;
        }
        // {   // testing
        //     printf("Remaining items.\n");
        //     print_itemset(itemset_ptr, NULL);
        //     printf("\n\n");
        // }

        /* Build candidate itemsets. */
        itemset_list_ptr = generate_sub_itemset(itemset_ptr, frequent_level);
        free_itemset_list(itemset_ptr);
        // {   // testing
        //     printf("Generate candidate itemsets.\n");
        //     Itemset *temp = itemset_list_ptr;
        //     while (temp) {
        //         print_itemset(temp, NULL);
        //         printf("\n");
        //         temp = temp->next;
        //     }
        //     printf("\n");
        // }

        /* Build the hash table based on the itemset list bulit in last step. */
        while (itemset_list_ptr) {
            Itemset *itemset_ptr = itemset_list_ptr;
            itemset_list_ptr = itemset_list_ptr->next;
            itemset_ptr->next = NULL;
            insert_itemset_into_hash(candidate_head, itemset_ptr, itemID_digit, 1);
        }
    }
    // printf("Generate hash table.\n");
    // print_hash(candidate_head);
}

void count_support_of_candidate(int itemID_digit)
{
    transaction_ptr = transaction_head;
    while (transaction_ptr) {
        Itemset *temp = new_itemset();
        Item *item_ptr = transaction_ptr->item_list_head;
        while (item_ptr) {
            insert_item_into_itemset(temp, item_ptr->itemID);
            item_ptr = item_ptr->next;
        }
        Itemset *transaction_sub_itemset = generate_sub_itemset(temp, frequent_level);
        free_itemset_list(temp);

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
    // printf("Count supports of candidates.\n");
    // print_hash(candidate_head);
}

void new_frequent_level()
{
    Frequent *new_frequent = (Frequent *)malloc(sizeof(Frequent));
    new_frequent->itemset_list_head = NULL;
    new_frequent->level = ++frequent_level;
    new_frequent->length = 0;
    new_frequent->next = NULL;
    if (!frequent_ptr)
        frequent_head = frequent_ptr = new_frequent;
    else
        frequent_ptr->next = new_frequent;
}

void generate_frequent_itemset(int support_count)
{
    Itemset *itemset_list_ptr = DFS_traversal(candidate_head);
    Itemset *frequent_list_ptr = NULL, *infrequent_list_ptr = NULL;
    int length = 0;
    while (itemset_list_ptr) {
        Itemset *temp = itemset_list_ptr;
        itemset_list_ptr = itemset_list_ptr->next;
        temp->next = NULL;
        if (temp->count < support_count)
            infrequent_list_ptr = insert_itemset_into_list(temp, infrequent_list_ptr);
        else {
            frequent_list_ptr = insert_itemset_into_list(temp, frequent_list_ptr);
            length++;
        }
    }
    frequent_ptr->itemset_list_head = frequent_list_ptr;
    frequent_ptr->length = length;
    free_itemset_list(infrequent_list_ptr);
    free_hash(candidate_head);

    // printf("Generate frequent itemsets.\n");
    // print_frequent_itemset(frequent_ptr);
}

void generate_rule(float min_confidence, FILE *out)
{
    frequent_ptr = frequent_head->next;
    while (frequent_ptr) {
        Itemset *itemset_ptr = frequent_ptr->itemset_list_head;
        while (itemset_ptr) {
            Itemset *target = new_itemset();
            Item *item_ptr = itemset_ptr->item_list_head;
            while (item_ptr) {
                insert_item_into_itemset(target, item_ptr->itemID);
                item_ptr = item_ptr->next;
            }

            Itemset *sub_itemset_head = NULL, *sub_itemset_ptr = NULL;
            for (int i = 1; i < frequent_ptr->level; ++i) {
                if (!sub_itemset_head)
                    sub_itemset_head = sub_itemset_ptr = generate_sub_itemset(target, i);
                else
                    sub_itemset_ptr->next = generate_sub_itemset(target, i);
                while (sub_itemset_ptr->next)
                    sub_itemset_ptr = sub_itemset_ptr->next;
            }
            free_itemset_list(target);

            sub_itemset_ptr = sub_itemset_head;
            while (sub_itemset_ptr) {
                int support = find_frequent_count(sub_itemset_ptr, sub_itemset_ptr->length);
                float confidence = itemset_ptr->count / (float)support;
                if (confidence >= min_confidence) {
                    Itemset *difference = get_difference_itemset(itemset_ptr, sub_itemset_ptr);
                    sub_itemset_ptr->length = difference->length = 0;
                    print_itemset(sub_itemset_ptr, out);
                    fprintf(out, " -> ");
                    print_itemset(difference, out);
                    fprintf(out, " | ");
                    fprintf(out, "confidence=%.4f, ", confidence);
                    fprintf(out, "support=%d\n", itemset_ptr->count);
                    free_itemset_list(difference);
                }
                Itemset *temp = sub_itemset_ptr;
                sub_itemset_ptr = sub_itemset_ptr->next;
                temp->next = NULL;
                free_itemset_list(temp);
            }
            
            itemset_ptr = itemset_ptr->next;
        }
        frequent_ptr = frequent_ptr->next;
    }
}

int find_frequent_count(Itemset *target, int level)
{
    Frequent *ptr = frequent_head;
    for (int i = 1; i < level; ++i)
        ptr = ptr->next;
    
    Itemset *itemset_ptr = ptr->itemset_list_head;
    while (!is_identical_itemset(itemset_ptr, target))
        itemset_ptr = itemset_ptr->next;

    return itemset_ptr->count;
}

void print_frequent_itemset(Frequent *target)
{
    Itemset *itemset_list_ptr = target->itemset_list_head;
    while (itemset_list_ptr) {
        print_itemset(itemset_list_ptr, NULL);
        printf("\n");
        itemset_list_ptr = itemset_list_ptr->next;
    }
    printf("\n");
}
