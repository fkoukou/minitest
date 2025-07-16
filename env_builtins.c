/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fakoukou <fakoukou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 06:18:32 by fakoukou          #+#    #+#             */
/*   Updated: 2025/07/16 06:18:33 by fakoukou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(t_env *env_list, char **args)
{
	if (args && args[1])
	{
		fprintf(stderr, "env: too many arguments\n");
		return (2);
	}
	if (!env_list)
		return (1);
	while (env_list)
	{
		if (env_list->value && env_list->value[0] != '\0')
			printf("%s=%s\n", env_list->key, env_list->value);
		env_list = env_list->next;
	}
	return (0);
}
