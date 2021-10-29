#include <stdio.h>
#include <stdlib.h>
#include "command.h"
#include "transaction.h"

int main(int argc, char *argv[])
{
    /* Handle the command and setup corresponding variables. */
    FILE *in, *out;
    float support, confidence;
    int status = command_handler(argc, argv, &in, &out, &support, &confidence);
    if (status == COMMAND_ERROR)
        return 0;
    
    
}
