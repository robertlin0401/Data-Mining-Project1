#ifndef TRANSACTION_H
#define TRANSACTION_H

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
Item *new_item(int itemID);

/* Create an empty transaction, and append it to the end of 
 * the linked list of transactions.
 */
void new_transaction();

/* Insert a new item into the linked list of items in
 * current transaction.
 */
void insert_item(int itemID);

/* Print out item list of each transaction, one 
 * transaction per line.
 */
void print_all_transactions();

#endif
