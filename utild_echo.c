/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utild_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fakoukou <fakoukou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 06:51:06 by fakoukou          #+#    #+#             */
/*   Updated: 2025/07/16 08:54:22 by fakoukou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_itoa(int n)
{
	char	*str;
	int		len = (n <= 0) ? 1 : 0;
	int		tmp = n;

	while (tmp)
	{
		tmp /= 10;
		len++;
	}
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	str[len--] = '\0';
	if (n == 0)
		str[0] = '0';
	if (n < 0)
	{
		str[0] = '-';
		n = -n;
	}
	while (n)
	{
		str[len--] = n % 10 + '0';
		n /= 10;
	}
	return (str);
}

char *expand_variable(const char *arg, int *i, t_env *env_list)
{
    int     start;
    char    *var;
    char    *val;

    if (arg[*i] == '?')  // Cas spécial pour $?
    {
        (*i)++;
        return (ft_itoa(g_exit_status));
    }

    start = *i;
    while (arg[*i] && (ft_isalnum(arg[*i]) || arg[*i] == '_'))
        (*i)++;

    if (*i == start)  // aucun caractère valide après $
        return (strdup("$"));

    var = strndup(&arg[start], *i - start);
    val = get_env_value(env_list, var);
    free(var);

    if (val)
        return (strdup(val));
    return (strdup(""));
}

char	*get_env_path(t_env *env_list)
{
	while (env_list)
	{
		if (strcmp(env_list->key, "PATH") == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

void print_export_reverse(t_env *env_list)
{
	if (!env_list)
		return;
	print_export_reverse(env_list->next);
	if (env_list->value == NULL)
		printf("declare -x %s\n", env_list->key);
	else
		printf("declare -x %s=\"%s\"\n", env_list->key, env_list->value);
}


int	is_valid_env_var_key(const char *key)
{
	int	i;

	i = 0;
	if (!key || !key[0])
		return (0);
	if (isdigit((unsigned char)key[0]))
		return (0);
	while (key[i])
	{
		if (!isalnum((unsigned char)key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
void set_env_var(t_env **env_list, const char *key, const char *value)
{
    t_env *tmp = *env_list;

    while (tmp)
    {
        if (strcmp(tmp->key, key) == 0)
        {
            if (value) // ne modifier la valeur que si elle est non NULL
            {
                free(tmp->value);
                tmp->value = strdup(value);
            }
            // sinon ne rien faire, garder la valeur existante
            return;
        }
        tmp = tmp->next;
    }
    // Ajouter nouvelle variable même si value == NULL
    t_env *new_node = malloc(sizeof(t_env));
    new_node->key = strdup(key);
    new_node->value = value ? strdup(value) : NULL;
    new_node->next = *env_list;
    *env_list = new_node;
}


