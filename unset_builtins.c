/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fakoukou <fakoukou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 06:32:09 by fakoukou          #+#    #+#             */
/*   Updated: 2025/07/16 08:48:30 by fakoukou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	unset_key(t_env **env_list, const char *key)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = *env_list;
	prev = NULL;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*env_list = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	builtin_unset(char **args, t_env **env_list)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	while (args[i])
	{
		if (!is_valid_env_var_key(args[i]) || strchr(args[i], '=') != NULL)
		{
			fprintf(stderr, "unset: `%s': not a valid identifier\n", args[i]);
			ret = 1;
		}
		else
			unset_key(env_list, args[i]);
		i++;
	}
	return (ret);
}
