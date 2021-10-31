#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdlib.h>

typedef struct item {
    int itemID;
    struct item *next;
} Item;

typedef struct transaction {
    Item *item_list_head;
    Item *item_list_ptr;
    int length;
    struct transaction *next;
} Transaction;

Transaction *transaction_head = NULL;
Transaction *transaction_ptr = NULL;

/* Allocate a new item and initialize its content. */
Item *new_item(int itemID)
{
    Item *item_ptr = (Item *)malloc(sizeof(Item));
    item_ptr->itemID = itemID;
    item_ptr->next = NULL;
    return item_ptr;
}

/* Create an empty transaction, and append it to the end of 
 * the linked list of transactions.
 */
void new_transaction()
{
    Transaction *new_transaction = (Transaction *)malloc(sizeof(Transaction));
    new_transaction->item_list_head = NULL;
    new_transaction->item_list_ptr = NULL;
    new_transaction->length = 0;
    new_transaction->next = NULL;
    if (!transaction_ptr) {
        transaction_head = transaction_ptr = new_transaction;
    } else {
        transaction_ptr->next = new_transaction;
        transaction_ptr = new_transaction;
    }
}

/* Insert a new item into the linked list of items in
 * current transaction.
 */
void insert_item(int itemID)
{
    Item *item_ptr = new_item(itemID);
    if (!transaction_ptr->item_list_head) {
        transaction_ptr->item_list_head = transaction_ptr->item_list_ptr = item_ptr;
    } else {
        transaction_ptr->item_list_ptr->next = item_ptr;
        transaction_ptr->item_list_ptr = item_ptr;
    }
    transaction_ptr->length++;
}

/* Print out item list of each transaction, one 
 * transaction per line.
 */
void print_all_transactions()
{
    transaction_ptr = transaction_head;
    while (transaction_ptr) {
        printf("%2d | ", transaction_ptr->length);
        transaction_ptr->item_list_ptr = transaction_ptr->item_list_head;
        while (transaction_ptr->item_list_ptr) {
            printf("%d ", transaction_ptr->item_list_ptr->itemID);
            if (transaction_ptr->item_list_ptr->next)
                transaction_ptr->item_list_ptr = transaction_ptr->item_list_ptr->next;
            else
                break;
        }
        printf("\n");
        if (transaction_ptr->next)
            transaction_ptr = transaction_ptr->next;
        else
            break;
    }
}

#endif
