#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Vérifie que la clé est valide (lettres, chiffres, underscore, ne commence pas par chiffre)
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

int builtin_unset(char **args, t_env **env_list)
{
    int i = 1;
    int ret = 0;

    while (args[i])
    {
        if (!is_valid_env_var_key(args[i]) || strchr(args[i], '=') != NULL)
        {
            fprintf(stderr, "unset: `%s': not a valid identifier\n", args[i]);
            ret = 1; // erreur détectée
        }
        else
        {
            // Suppression de la variable dans la liste chaînée
            t_env *tmp = *env_list;
            t_env *prev = NULL;
            while (tmp)
            {
                if (strcmp(tmp->key, args[i]) == 0)
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
