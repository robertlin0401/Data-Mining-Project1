#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdlib.h>

typedef struct item {
    int itemID;
    struct item *next;
} Item;

typedef struct transaction {
    Item *item_list_head;
    Item *item_list_tail;
    struct transaction *next;
} Transaction;

Transaction *head = NULL;
Transaction *ptr = NULL;

/* Create an empty transaction, and append it to the end of 
 * the linked list of transactions.
 */
void add_transaction()
{
    Transaction *new_transaction = (Transaction *)malloc(sizeof(Transaction));
    new_transaction->item_list_head = NULL;
    new_transaction->item_list_tail = NULL;
    new_transaction->next = NULL;
    if (!ptr) {
        head = ptr = new_transaction;
    } else {
        ptr->next = new_transaction;
        ptr = new_transaction;
    }
}

/* Insert a new item into the linked list of items in
 * current transaction.
 */
void insert_item(int itemID)
{
    Item *new_item = (Item *)malloc(sizeof(Item));
    new_item->itemID = itemID;
    new_item->next = NULL;
    if (!ptr->item_list_head) {
        ptr->item_list_head = ptr->item_list_tail = new_item;
    } else {
        ptr->item_list_tail->next = new_item;
        ptr->item_list_tail = new_item;
    }
}

/* Print out item list of each transaction, one 
 * transaction per line.
 */
void print_all_transactions()
{
    ptr = head;
    while (ptr) {
        ptr->item_list_tail = ptr->item_list_head;
        while (ptr->item_list_tail) {
            printf("%d ", ptr->item_list_tail->itemID);
            if (ptr->item_list_tail->next)
                ptr->item_list_tail = ptr->item_list_tail->next;
            else
                break;
        }
        printf("\n");
        if (ptr->next)
            ptr = ptr->next;
        else
            break;
    }
}

#endif
