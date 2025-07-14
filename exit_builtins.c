#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

static bool is_numeric_argument(const char *arg)
{
    int i = 0;

    if (!arg)
        return false;

    while (arg[i] && isspace((unsigned char)arg[i]))
        i++;

    if (arg[i] == '+' || arg[i] == '-')
        i++;

    if (!isdigit((unsigned char)arg[i]))
        return false;

    while (arg[i])
    {
        if (!isdigit((unsigned char)arg[i]))
            return false;
        i++;
    }
    return true;
}

int builtin_exit(char **args)
{
    long exit_code = 0;

    printf("exit\n");

    if (!args[1])
        exit(0);

    if (!is_numeric_argument(args[1]))
    {
        fprintf(stderr, "exit: %s: numeric argument required\n", args[1]);
        exit(255);
    }

    exit_code = strtol(args[1], NULL, 10);

    if (args[2])
    {
        fprintf(stderr, "exit: too many arguments\n");
        return 1; 
    }

    exit(exit_code % 256);
}
