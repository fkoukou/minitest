#include "minishell.h"
#include <stdio.h>
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
#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// Expansion d'une variable $VAR
char *expand_arg(char *arg, t_env *env_list)
{
    char *result = malloc(1);
    result[0] = '\0';
    int i = 0;
    while (arg[i])
    {
        if (arg[i] == '$')
        {
            int start = ++i;
            while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
                i++;
            char *var = strndup(&arg[start], i - start);
            char *val = get_env_value(env_list, var);
            result = ft_strjoin_free(result, val ? val : "");
            free(var);
        }
        else
        {
            char tmp[2] = { arg[i++], '\0' };
            result = ft_strjoin_free(result, tmp);
        }
    }
    return result;
}

// Version avec free de s1
char *ft_strjoin_free(char *s1, const char *s2)
{
    char *joined;
    int len1 = strlen(s1), len2 = strlen(s2);
    joined = malloc(len1 + len2 + 1);
    if (!joined) return NULL;
    strcpy(joined, s1);
    strcat(joined, s2);
    free(s1);
    return joined;
}

// Fonction echo avec prise en compte de l’expansion
int builtin_echo(char **args, t_env *env_list)
{
    int i = 1;
    bool n_flag = false;

    while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
    {
        int j = 1;
        while (args[i][j] == 'n')
            j++;
        if (args[i][j] != '\0')
            break;
        n_flag = true;
        i++;
    }

    while (args[i])
    {
        char *expanded =       expand_arg(args[i], env_list);
        printf("%s", expanded);
        if (args[i + 1])
            printf(" ");
        free(expanded);
        i++;
    }

    if (!n_flag)
        printf("\n");

    return 0;
}
