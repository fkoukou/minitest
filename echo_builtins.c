#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// static bool is_n_flag(const char *arg)
// {
//     int i = 0;
//     if (!arg || arg[0] != '-')
//         return false;
//     i++;
//     while (arg[i] == 'n')
//         i++;
//     return arg[i] == '\0';
// }
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Cette fonction doit parser input en tokens en respectant les guillemets
// et en gardant les espaces dans les guillemets.

char **tokenize_echo_args(char *input)
{
    char **args = malloc(sizeof(char *) * 256); // taille max arbitraire
    int argc = 0;
    int i = 0, start = 0;
    bool in_single_quote = false, in_double_quote = false;

    while (input[i])
    {
        if (input[i] == '\'' && !in_double_quote)
            in_single_quote = !in_single_quote;
        else if (input[i] == '"' && !in_single_quote)
            in_double_quote = !in_double_quote;
        else if (!in_single_quote && !in_double_quote && input[i] == ' ')
        {
            if (i > start)
            {
                int len = i - start;
                char *arg = malloc(len + 1);
                strncpy(arg, &input[start], len);
                arg[len] = '\0';
                args[argc++] = arg;
            }
            start = i + 1;
        }
        i++;
    }

    if (i > start)
    {
        int len = i - start;
        char *arg = malloc(len + 1);
        strncpy(arg, &input[start], len);
        arg[len] = '\0';
        args[argc++] = arg;
    }

    args[argc] = NULL;
    return args;
}

int builtin_echo(char **args)
{
    int i = 1;
    bool n_flag = false;

    // Gestion du flag -n (optionnel)
    while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
    {
        int j = 1;
        while (args[i][j] == 'n')
            j++;
        if (args[i][j] != '\0')
            break;  // Pas un flag -n valide, sortir
        n_flag = true;
        i++;
    }

    // Affichage des arguments tels quels, séparés par un espace simple
    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }

    // Si pas de flag -n, affiche un saut de ligne
    if (!n_flag)
        printf("\n");

    return 0;
}

