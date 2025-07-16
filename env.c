/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fakoukou <fakoukou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 06:19:50 by fakoukou          #+#    #+#             */
/*   Updated: 2025/07/16 06:59:17 by fakoukou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

static t_env	*create_node(char *env_line)
{
	t_env	*new;
	char	**split;

	split = ft_split(env_line, '=');
	if (!split || !split[0])
	{
		free_array(split);
		return (NULL);
	}
	new = malloc(sizeof(t_env));
	if (!new)
	{
		free_array(split);
		return (NULL);
	}
	new->key = strdup(split[0]);
	if (split[1])
		new->value = strdup(split[1]);
	else
		new->value = strdup("");
	new->next = NULL;
	free_array(split);
	return (new);
}

t_env	*create_env(char **envp)
{
	t_env	*head;
	t_env	*new_node;
	int		i;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = create_node(envp[i]);
		if (new_node)
		{
			new_node->next = head;
			head = new_node;
		}
		i++;
	}
	return (head);
}

void	print_env(t_env *env)
{
	while (env)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}
