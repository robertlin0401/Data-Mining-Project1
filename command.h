#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define COMMAND_ERROR 0
#define COMMAND_CORRECT 1

int help_command(char *argv[], char *err_msg)
{
    printf("%s\n", err_msg);
    printf("Usage:\n");
    printf("  execute | %s -i <input> [-o <output>]\n", argv[0]);
    printf("  help    | %s -h\n", argv[0]);
    printf("Options:\n");
    printf("  -h, --help   | Show usages and list available options.\n");
    printf("  -i, --input  | Specify the input directory. (required)\n");
    printf("  -o, --output | Specify the output directory. (defalut: output.txt)\n");
    return COMMAND_ERROR;
}

int command_handler(int argc, char *argv[], FILE **in, FILE **out)
{
    /* Check if there is -h or --help flag. */
    for (int i = 1; i < argc; ++i) 
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
            return help_command(argv, NULL);

    /* There exists unpaired flag or value, or there is no flag. */
    if (argc % 2 == 0 || argc == 1)
        return help_command(argv, "Wrong format! Please check out your command.");

    /* Process the command and setup corresponding variables. */
    bool has_flag_used[2] = {0, 0}; // {-i, -o}
    for (int i = 1; i < argc; i += 2) {
        char *flag = argv[i];
        char *value = argv[i + 1];
        if (strcmp(flag, "-i") == 0 || strcmp(flag, "--input") == 0) {

            if (!has_flag_used[0])
                has_flag_used[0] = 1;
            else
                return help_command(argv, "Multiple input options! Please check out your command.");
            
            *in = fopen(value, "r");
            if (!*in)
                return help_command(argv, "Input directory not exists! Please check out your command.");

        } else if (strcmp(flag, "-o") == 0 || strcmp(flag, "--output") == 0) {

            if (!has_flag_used[1])
                has_flag_used[1] = 1;
            else
                return help_command(argv, "Multiple output options! Please check out your command.");
            
            *out = fopen(value, "w");

        } else {
            return help_command(argv, NULL);
        }
    }
    if (!has_flag_used[0])
        return help_command(argv, "No input directory! Please check out your command.");
    if (!has_flag_used[1])
        *out = fopen("output.txt", "w");
    
    return COMMAND_CORRECT;
}

#endif
