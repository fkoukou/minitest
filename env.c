#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "minishell.h" 

static int count_words(const char *s, char c)
{
    int count = 0;
    int i = 0;

    while (s[i])
    {
        while (s[i] && s[i] == c)
            i++;
        if (s[i] && s[i] != c)
            count++;
        while (s[i] && s[i] != c)
            i++;
    }
    return count;
}

static char *word_dup(const char *s, int start, int finish)
{
    char *word = malloc(finish - start + 1);
    int i = 0;
    while (start < finish)
        word[i++] = s[start++];
    word[i] = '\0';
    return word;
}

char **ft_split(char const *s, char c)
{
    char **result;
    int i = 0, j = 0, index = -1;

    if (!s || !(result = malloc((count_words(s, c) + 1) * sizeof(char *))))
        return NULL;

    while (s[i])
    {
        if (s[i] != c && index < 0)
            index = i;
        else if ((s[i] == c || s[i + 1] == '\0') && index >= 0)
        {
            result[j++] = word_dup(s, index, (s[i] == c ? i : i + 1));
            index = -1;
        }
        i++;
    }
    result[j] = NULL;
    return result;
}

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
        free_array(split); // une fonction pour free le tableau split
        i++;
    }
    return head;
}

// Prototype et définition
// char *get_env_value(t_env *env_list, const char *key)
// {
//     while (env_list)
//     {
//         if (strcmp(env_list->key, key) == 0)
//             return env_list->value;
//         env_list = env_list->next;
//     }
//     return NULL;
// }


void print_env(t_env *env)
{
    while (env)
    {
        printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
}
