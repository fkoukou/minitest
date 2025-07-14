#include "minishell.h"

static int is_valid_env_var_key(const char *key)
{
    int i = 0;

    if (!key || !key[0])
        return 0;
    if (ft_isdigit((unsigned char)key[0]))
        return 0;
    while (key[i])
    {
        if (!ft_isalnum((unsigned char)key[i]) && key[i] != '_')
            return 0;
        i++;
    }
    return 1;
}

int builtin_unset(char **args, t_env **env_list)
{
    int i = 1;
    int ret = 0;

    while (args[i])
    {
        if (!is_valid_env_var_key(args[i]) || strchr(args[i], '=') != NULL)
        {
            fprintf(stderr, "unset: `%s': not a valid identifier\n", args[i]);
            ret = 1; 
        }
        else
        {
            t_env *tmp = *env_list;
            t_env *prev = NULL;
            while (tmp)
            {
                if (ft_strcmp(tmp->key, args[i]) == 0)
                {
                    if (prev)
                        prev->next = tmp->next;
                    else
                        *env_list = tmp->next;
                    free(tmp->key);
                    free(tmp->value);
                    free(tmp);
                    break;
                }
                prev = tmp;
                tmp = tmp->next;
            }
        }
        i++;
    }
    return ret;
}
