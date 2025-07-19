/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fakoukou <fakoukou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 08:39:44 by fakoukou          #+#    #+#             */
/*   Updated: 2025/07/17 20:00:43 by fakoukou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_prev_fd(int prev_fd)
{
	if (prev_fd != -1)
		close(prev_fd);
}

void	child_redirects(int prev_fd, int pipefd[2], size_t i, size_t nb_cmd)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (i < nb_cmd - 1)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
}

static void	wait_children(size_t nb_cmd)
{
	size_t	i;

	i = 0;
	while (i < nb_cmd)
	{
		wait(NULL);
		i++;
	}
}


void	handle_pipe_and_prev_fd(long unsigned int i, size_t nb_cmd,
		int pipefd[2], int *prev_fd)
{
	if (i < nb_cmd - 1)
	{
		close(pipefd[1]);
		if (*prev_fd != -1)
			close(*prev_fd);
		*prev_fd = pipefd[0];
	}
	else if (*prev_fd != -1)
	{
		close(*prev_fd);
	}
}
static void	process_command(t_process_args *args)
{
	int				pipefd[2];
	t_prepare_args	prepare;

	if (args->i < args->nb_cmd - 1 && pipe(pipefd) == -1)
		exit(1);
	prepare.env_list = args->env_list;
	prepare.cmd = args->cmd;
	prepare.prev_fd = *(args->prev_fd);
	prepare.pipefd[0] = pipefd[0];
	prepare.pipefd[1] = pipefd[1];
	prepare.i = args->i;
	prepare.nb_cmd = args->nb_cmd;
	prepare_params(args->params, &prepare);
	fork_and_exec(args->params);
	handle_pipe_and_prev_fd(args->i, args->nb_cmd, pipefd, args->prev_fd);
}

void	pipeline(t_env **env_list, t_command *cmd_list, size_t nb_cmd)
{
	size_t				i;
	int					prev_fd;
	t_fork_exec_params	params;
	t_process_args		args;

	i = 0;
	prev_fd = -1;
	args.env_list = env_list;
	args.params = &params;
	args.prev_fd = &prev_fd;
	args.nb_cmd = nb_cmd;
	while (cmd_list)
	{
		args.cmd = cmd_list;
		args.i = i;
		process_command(&args);
		cmd_list = cmd_list->next_pipe;
		i++;
	}
	wait_children(nb_cmd);
}

