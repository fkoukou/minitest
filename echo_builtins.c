#include "minishell.h"
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
