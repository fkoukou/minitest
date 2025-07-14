#include "minishell.h"

int builtin_env(t_env *env_list, char **args)
{
    if (args && args[1])
    {
        fprintf(stderr, "env: too many arguments\n");
        return 2; 
    }
    if (!env_list)
        return 1; 

    while (env_list)
    {
        if (env_list->value && env_list->value[0] != '\0')
            printf("%s=%s\n", env_list->key, env_list->value);
        env_list = env_list->next;
    }
    return 0;
}
