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

Itemset *insert_itemset_into_list(Itemset *target, Itemset *list)
{
    if (!list)
        return target;
    else {
        Itemset *temp = list;
        while (temp->next)
            temp = temp->next;
        temp->next = target;
        return list;
    }
}

Itemset *generate_sub_itemset(Itemset *itemset_ptr, int length)
{
    Itemset *result = NULL, *ptr = NULL;
    if (length == 1) {
        Item *item_list_ptr = itemset_ptr->item_list_head;
        while (item_list_ptr) {
            Itemset *temp = new_itemset();
            insert_item_into_itemset(temp, item_list_ptr->itemID);
            if (!ptr) {
                result = ptr = temp;
            } else {
                ptr->next = temp;
                ptr = temp;
            }
            item_list_ptr = item_list_ptr->next;
        }
    } else {
        while (itemset_ptr->length >= length) {
            Item *item_head = itemset_ptr->item_list_head;
            itemset_ptr->item_list_head = itemset_ptr->item_list_head->next;
            itemset_ptr->length--;

            Itemset *target = new_itemset();
            Item *item_ptr = item_head->next;
            while (item_ptr) {
                insert_item_into_itemset(target, item_ptr->itemID);
                item_ptr = item_ptr->next;
            }

            Itemset *sub_itemset_ptr = generate_sub_itemset(target, length - 1);
            while (sub_itemset_ptr) {
                insert_item_into_itemset(sub_itemset_ptr, item_head->itemID);
                if (!ptr) {
                    result = ptr = sub_itemset_ptr;
                } else {
                    ptr->next = sub_itemset_ptr;
                    ptr = sub_itemset_ptr;
                }
                sub_itemset_ptr = sub_itemset_ptr->next;
            }

            free_itemset_list(target);
            free(item_head);
        }
    }
    return result;
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

bool is_identical_itemset(Itemset *target, Itemset *comparison)
{
    return target->length == comparison->length && is_sub_itemset(target, comparison);
}

void free_itemset_list(Itemset *target)
{
    while (target) {
        Item *item_list_head = target->item_list_head;
        while (item_list_head) {
            Item *temp = item_list_head->next;
            free(item_list_head);
            item_list_head = temp;
        }
        Itemset *temp = target->next;
        free(target);
        target = temp;
    }
}

void print_itemset(Itemset *target)
{
    Item *temp = target->item_list_head;
    if (target->count)
        printf("%4d | ", target->count);
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
    if (target->length)
        printf(" -> %2d", target->length);
}
