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

bool is_sub_itemset(Itemset *target, Itemset *comparison)
{
    int length_diff = comparison->length - target->length;
    if (length_diff < 0)
        return 0;

    Item *target_item_list = target->item_list_head;
    Item *comparison_item_list = comparison->item_list_head;
    while (comparison_item_list) {
        if (comparison_item_list->itemID == target_item_list->itemID) {
            target_item_list = target_item_list->next;
        } else {
            length_diff--;
        }

        if (length_diff < 0)
            return 0;
        if (!target_item_list)
            return 1;

        comparison_item_list = comparison_item_list->next;
    }
}

void print_itemset(Itemset *target)
{
    Item *temp = target->item_list_head;
    printf("{");
    while (1) {
        printf("%d", temp->itemID);
        temp = temp->next;
        if (temp)
            printf(", ");
        else
            break;
    }
    printf("}");
}
