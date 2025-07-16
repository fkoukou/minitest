/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fakoukou <fakoukou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 06:44:47 by fakoukou          #+#    #+#             */
/*   Updated: 2025/07/16 06:54:50 by fakoukou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*append_char(char *res, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	return (ft_strjoin_free(res, tmp));
}

char	*expand_arg(char *arg, t_env *env_list)
{
	char	*result;
	int		i;

	result = malloc(1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	i = 0;
	while (arg[i])
	{
		if (arg[i] == '$')
		{
			i++;
			result = ft_strjoin_free(result, expand_variable(arg, &i,
						env_list));
		}
		else
		{
			result = append_char(result, arg[i]);
			i++;
		}
	}
	return (result);
}

static bool	has_n_flag(char *arg)
{
	int	j;

	if (arg[0] != '-' || arg[1] != 'n')
		return (false);
	j = 2;
	while (arg[j])
	{
		if (arg[j] != 'n')
			return (false);
		j++;
	}
	return (true);
}

static void	print_args(char **args, int start, t_env *env_list)
{
	char	*expanded;

	while (args[start])
	{
		expanded = expand_arg(args[start], env_list);
		printf("%s", expanded);
		if (args[start + 1])
			printf(" ");
		free(expanded);
		start++;
	}
}

int	builtin_echo(char **args, t_env *env_list)
{
	int		i;
	bool	n_flag;

	i = 1;
	n_flag = false;
	while (args[i] && has_n_flag(args[i]))
	{
		n_flag = true;
		i++;
	}
	print_args(args, i, env_list);
	if (!n_flag)
		printf("\n");
	return (0);
}
