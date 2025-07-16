/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fakoukou <fakoukou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 06:21:13 by fakoukou          #+#    #+#             */
/*   Updated: 2025/07/16 06:21:19 by fakoukou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_numeric_argument(const char *arg)
{
	int	i;

	i = 0;
	if (!arg)
		return (false);
	while (arg[i] && isspace((unsigned char)arg[i]))
		i++;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (!isdigit((unsigned char)arg[i]))
		return (false);
	while (arg[i])
	{
		if (!isdigit((unsigned char)arg[i]))
			return (false);
		i++;
	}
	return (true);
}

int	builtin_exit(char **args)
{
	long	exit_code;

	exit_code = 0;
	printf("exit\n");
	if (!args[1])
		exit(0);
	if (!is_numeric_argument(args[1]))
	{
		fprintf(stderr, "exit: %s: numeric argument required\n", args[1]);
		exit(255);
	}
	exit_code = strtol(args[1], NULL, 10);
	if (args[2])
	{
		fprintf(stderr, "exit: too many arguments\n");
		return (1);
	}
	exit(exit_code % 256);
}
