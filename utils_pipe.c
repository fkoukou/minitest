/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fakoukou <fakoukou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 08:22:38 by fakoukou          #+#    #+#             */
/*   Updated: 2025/07/16 08:59:26 by fakoukou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_exit_status(int id)
{
	int	res;

	waitpid(id, &res, 0);
	if (WIFSIGNALED(res))
	{
		return (130);
	}
	return (WEXITSTATUS(res));
}

void	test(char **args, t_env *env_list)
{
	char	*cmd_path;
	char	**envp;

	cmd_path = find_in_path(args[0], env_list);
	if (!cmd_path)
	{
		printf("Command not found: %s\n", args[0]);
		return ;
	}
	envp = env_list_to_array(env_list);
	execve(cmd_path, args, envp);
	perror("execve");
	exit(EXIT_FAILURE);
}

static void	handle_fork_failure(void)
{
	perror("fork");
	exit(1);
}

void fork_and_exec(t_fork_exec_params *params)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        child_redirects(params->prev_fd, params->pipefd, params->i, params->nb_cmd, params->cmd);

        if (execute_builtins(params->env_list, params->cmd) == CMD_NOT_FOUND)
        {
            if (execvp(params->cmd->args[0], params->cmd->args) == -1)
            {
                perror(params->cmd->args[0]);
                exit(EXIT_FAILURE);
            }
        }
        exit(EXIT_SUCCESS);
    }
    else if (pid < 0)
        handle_fork_failure();
}



void	prepare_params(t_fork_exec_params *params, t_prepare_args *args)
{
	params->env_list = args->env_list;
	params->cmd = args->cmd;
	params->prev_fd = args->prev_fd;
	params->pipefd[0] = args->pipefd[0];
	params->pipefd[1] = args->pipefd[1];
	params->i = args->i;
	params->nb_cmd = args->nb_cmd;
}
