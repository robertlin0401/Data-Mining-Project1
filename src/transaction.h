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

Transaction *transaction_head = NULL;
Transaction *transaction_ptr = NULL;

/* Create an empty transaction, and append it to the end of 
 * the linked list of transactions.
 */
void new_transaction()
{
    Transaction *new_transaction = (Transaction *)malloc(sizeof(Transaction));
    new_transaction->item_list_head = NULL;
    new_transaction->item_list_tail = NULL;
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
    Item *new_item = (Item *)malloc(sizeof(Item));
    new_item->itemID = itemID;
    new_item->next = NULL;
    if (!transaction_ptr->item_list_head) {
        transaction_ptr->item_list_head = transaction_ptr->item_list_tail = new_item;
    } else {
        transaction_ptr->item_list_tail->next = new_item;
        transaction_ptr->item_list_tail = new_item;
    }
}

/* Print out item list of each transaction, one 
 * transaction per line.
 */
void print_all_transactions()
{
    transaction_ptr = transaction_head;
    while (transaction_ptr) {
        transaction_ptr->item_list_tail = transaction_ptr->item_list_head;
        while (transaction_ptr->item_list_tail) {
            printf("%d ", transaction_ptr->item_list_tail->itemID);
            if (transaction_ptr->item_list_tail->next)
                transaction_ptr->item_list_tail = transaction_ptr->item_list_tail->next;
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
