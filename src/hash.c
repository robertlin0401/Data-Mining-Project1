#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "transaction.h"
#include "hash.h"

const int max_leaf_size = 50;

Hash *new_hash()
{
    Hash *hash_ptr = (Hash *)malloc(sizeof(Hash));
    for (int i = 0; i < 10; ++i) {
        hash_ptr->ptr[i] = NULL;
        hash_ptr->length[i] = 0;
        hash_ptr->is_hash[i] = 0;
    }
    return hash_ptr;
}

void split_itemset_to_hash(Itemset *target, Hash *hash_ptr, int itemID_digit, int level)
{
    Itemset *temp = target;
    while (target) {
        temp = temp->next;
        target->next = NULL;
        insert_itemset_into_hash(hash_ptr, target, itemID_digit, level);
        target = temp;
    }
}

void insert_itemset_into_hash(Hash *target, Itemset *itemset_ptr, int itemID_digit, int level)
{
    Item *item_ptr = itemset_ptr->item_list_head;
    for (int i = 1; i < level; ++i)
        item_ptr = item_ptr->next;

    int hash_value = item_ptr->itemID / pow(10, itemID_digit);
    if (target->is_hash[hash_value]) {
        insert_itemset_into_hash((Hash *)target->ptr[hash_value], itemset_ptr, itemID_digit, level + 1);
    } else {
        Itemset *temp = (Itemset *)target->ptr[hash_value];
        if (target->length[hash_value] == max_leaf_size && itemset_ptr->length > level) {
            target->ptr[hash_value] = new_hash();
            target->length[hash_value] = 0;
            target->is_hash[hash_value] = 1;
            split_itemset_to_hash(temp, (Hash *)target->ptr[hash_value], itemID_digit, level + 1);
            insert_itemset_into_hash((Hash *)target->ptr[hash_value], itemset_ptr, itemID_digit, level + 1);
        } else {
            if (!temp) {
                target->ptr[hash_value] = itemset_ptr;
            } else {
                while (temp->next)
                    temp = temp->next;
                temp->next = itemset_ptr;
            }
            target->length[hash_value]++;
        }
    }
}
