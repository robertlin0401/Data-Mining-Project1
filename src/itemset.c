#include <stdio.h>
#include <stdlib.h>
#include "itemset.h"

Itemset *new_itemset()
{
    Itemset *itemset_ptr = (Itemset *)malloc(sizeof(Itemset));
    itemset_ptr->item_list_head = NULL;
    itemset_ptr->count = 0;
    itemset_ptr->length = 0;
    itemset_ptr->next = NULL;
    return itemset_ptr;
}

void insert_item_into_itemset(Itemset *target, int itemID)
{
    Item *item_ptr = new_item(itemID);
    Item *temp_ptr = target->item_list_head;
    if (!temp_ptr) {
        target->item_list_head = item_ptr;
    } else {
        if (temp_ptr->itemID > itemID) {
            item_ptr->next = temp_ptr;
            target->item_list_head = item_ptr;
        } else {
            while (temp_ptr->next) {
                if (temp_ptr->next->itemID > itemID)
                    break;
                temp_ptr = temp_ptr->next;
            }
            item_ptr->next = temp_ptr->next;
            temp_ptr->next = item_ptr;
        }
    }
    target->length++;
}
