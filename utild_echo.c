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

char	*expand_variable(const char *arg, int *i, t_env *env_list)
{
	int		start;
	char	*var;
	char	*val;

	start = *i;
	while (arg[*i] && (ft_isalnum(arg[*i]) || arg[*i] == '_'))
		(*i)++;
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
    t_env *tmp;
    t_env *new_node;

    tmp = *env_list;
    while (tmp)
    {
        if (strcmp(tmp->key, key) == 0)
        {
            free(tmp->value);
            if (value)
                tmp->value = strdup(value);
            else
                tmp->value = NULL;  
            return;
        }
        tmp = tmp->next;
    }
    new_node = malloc(sizeof(t_env));
    new_node->key = strdup(key);
    if (value)
        new_node->value = strdup(value);
    else
        new_node->value = NULL;  // garder NULL ici
    new_node->next = *env_list;
    *env_list = new_node;
}

