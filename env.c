#include "minishell.h" 



void free_array(char **array)
{
    int i = 0;
    while (array[i])
        free(array[i++]);
    free(array);
}

void free_env(t_env *env)
{
    t_env *tmp;
    while (env)
    {
        tmp = env;
        env = env->next;
        free(tmp->key);
        free(tmp->value);
        free(tmp);
    }
}

t_env *create_env(char **envp)
{
    t_env *head = NULL;
    t_env *new;
    char **split;
    int i = 0;

    while (envp[i])
    {
        split = ft_split(envp[i], '='); // si tu utilises ta fonction ft_split()
        if (!split || !split[0])
        {
            i++;
            continue;
        }
        new = malloc(sizeof(t_env));
        new->key = strdup(split[0]);
        new->value = strdup(split[1] ? split[1] : "");
        new->next = head;
        head = new;
        free_array(split); 
        i++;
    }
    return head;
}

void print_env(t_env *env)
{
    while (env)
    {
        printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
}
