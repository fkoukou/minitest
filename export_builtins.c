/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fakoukou <fakoukou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 06:24:01 by fakoukou          #+#    #+#             */
/*   Updated: 2025/07/16 07:59:08 by fakoukou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_key_value(char *key, char *value)
{
	free(key);
	if (value)
		free(value);
}

static void	extract_key_value(char *arg, char **key, char **value)
{
	char	*equal;
	int		key_len;

	equal = strchr(arg, '=');
	if (equal)
	{
		key_len = equal - arg;
		*key = strndup(arg, key_len);
		*value = strdup(equal + 1);
	}
	else
	{
		*key = strdup(arg);
		*value = NULL;
	}
}

static void	handle_export_arg(char *arg, t_env **env_list)
{
	char	*key;
	char	*value;

	extract_key_value(arg, &key, &value);
	if (!is_valid_env_var_key(key))
	{
		fprintf(stderr, "export: `%s': not a valid identifier\n", key);
		free_key_value(key, value);
		return ;
	}
	set_env_var(env_list, key, value);
	free_key_value(key, value);
}

int	builtin_export(char **args, t_env **env_list)
{
	int	i;

	if (!args[1])
	{
		print_export_reverse(*env_list);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		handle_export_arg(args[i], env_list);
		i++;
	}
	return (0);
}
