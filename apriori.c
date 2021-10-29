#include <stdio.h>
#include <stdlib.h>
#include "command.h"
#include "transaction.h"

int main(int argc, char *argv[])
{
    FILE *in, *out;
    int status = command_handler(argc, argv, &in, &out);
    if (status == COMMAND_ERROR)
        return 0;
    
    
}
