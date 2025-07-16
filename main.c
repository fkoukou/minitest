/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fakoukou <fakoukou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 08:04:41 by fakoukou          #+#    #+#             */
/*   Updated: 2025/07/16 08:05:34 by fakoukou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_command(char *input, t_env **env_list)
{
	t_token		*tokens;
	t_command	*cmd_list;
	size_t		nb_cmd;
	int			fd_in;
	int			fd_out;

	tokens = tokenize(input, *env_list);
	if (!tokens)
		return ;
	cmd_list = parse_pipeline(&tokens);
	if (!cmd_list)
		return ;
	nb_cmd = size_cmd(cmd_list);
	fd_in = dup(STDIN_FILENO);
	fd_out = dup(STDOUT_FILENO);
	if (nb_cmd == 1)
	{
		if (execute_builtins(env_list, cmd_list) == CMD_NOT_FOUND)
			execute_external(cmd_list->args, *env_list);
	}
	else
		pipeline(env_list, cmd_list, nb_cmd);
	dup2(fd_in, STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env_list;
	char	*input;

	(void)argc;
	(void)argv;
	env_list = create_env(envp);
	setup_signal_handlers();
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		if (input[0] == '\0')
		{
			free(input);
			continue ;
		}
		add_history(input);
		handle_command(input, &env_list);
		free(input);
	}
	printf("exit\n");
	free_env(env_list);
	return (0);
}
