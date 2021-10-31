#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "command.h"
#include "transaction.h"
#include "frequent.h"

int main(int argc, char *argv[])
{
    /* Handle the command and setup corresponding variables. */
    FILE *in, *out;
    float support, confidence;
    int status = command_handler(argc, argv, &in, &out, &support, &confidence);
    if (status == COMMAND_ERROR)
        return 0;

    /* Construct transactions' data structure. */
    int customerID, transactionID, itemID;
    int transaction_count = 0, max_itemID = 0;
    while (fscanf(in, "%d %d %d", &customerID, &transactionID, &itemID) != EOF) {
        if (transactionID != transaction_count) {
            transaction_count = transactionID;
            new_transaction();
        }
        if (itemID > max_itemID)
            max_itemID = itemID;
        insert_item(itemID);
    }
    // print_all_transactions();

    /*============================================================
     *                    Apriori Algorithm
     *============================================================
     */
    /* Find frequent itemsets. */
    int support_count = ceil(transaction_count * support);
    int itemID_digit = 0, temp = max_itemID;
    while (temp / 10 > 0) {
        temp /= 10;
        itemID_digit++;
    }
    generate_candidate(support_count, itemID_digit);

    return 0;
}
