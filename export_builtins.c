#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int is_valid_env_var_key(const char *key)
{
    int i = 0;

    if (!key || !key[0])
        return 0;
    if (isdigit((unsigned char)key[0]))
        return 0;
    while (key[i])
    {
        if (!isalnum((unsigned char)key[i]) && key[i] != '_')
            return 0;
        i++;
    }
    return 1;
}

void set_env_var(t_env **env_list, const char *key, const char *value)
{
    t_env *tmp = *env_list;

    while (tmp)
    {
        if (strcmp(tmp->key, key) == 0)
        {
            free(tmp->value);
            tmp->value = strdup(value ? value : "");
            return;
        }
        tmp = tmp->next;
    }
    t_env *new_node = malloc(sizeof(t_env));
    new_node->key = strdup(key);
    new_node->value = strdup(value ? value : "");
    new_node->next = *env_list;
    *env_list = new_node;
}

void print_export_reverse(t_env *env_list)
{
    if (!env_list)
        return;
    print_export_reverse(env_list->next);
    printf("declare -x %s=\"%s\"\n", env_list->key, env_list->value);
}

int builtin_export(char **args, t_env **env_list)
{
    if (!args[1])
    {
        print_export_reverse(*env_list);
        return 0;
    }

    for (int i = 1; args[i]; i++)
    {
        char *equal = strchr(args[i], '=');
        char *key;
        char *value;

        if (equal)
        {
            int key_len = equal - args[i];
            key = strndup(args[i], key_len);
            value = strdup(equal + 1);
        }
        else
        {
            key = strdup(args[i]);
            value = NULL;
        }

        if (!is_valid_env_var_key(key))
        {
            fprintf(stderr, "export: `%s': not a valid identifier\n", key);
            free(key);
            free(value);
            continue;
        }

        set_env_var(env_list, key, value);

        free(key);
        free(value);
    }
    return 0;
}
