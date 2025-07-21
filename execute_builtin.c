/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fakoukou <fakoukou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 06:20:12 by fakoukou          #+#    #+#             */
/*   Updated: 2025/07/21 09:56:22 by fakoukou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void handle_redirections(t_redirect *redirects, t_env *env_list)
{
	(void)env_list;
    while (redirects)
    {
        if (redirects->type == T_REDIR_IN)
            rediriger_entree(redirects->filename);
        else if (redirects->type == T_REDIR_OUT)
            rediriger_sortie(redirects->filename);
        else if (redirects->type == T_REDIR_APPEND)
            rediriger_sortie_append(redirects->filename);
		else if (redirects->type == T_HEREDOC)
            rediriger_heredoc(redirects->filename);
        redirects = redirects->next;
    }
}
void rediriger_heredoc(const char *fichier)
{
	int	fd;
	fd = open(fichier, O_RDONLY);
	if (fd < 0)
	{
		perror("open heredoc");
		return ;
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}


int	execute_builtins_no_redir(t_env **env_list, t_command *cmd)
{
	if (strcmp(cmd->args[0], "cd") == 0)
		return (builtin_cd(*env_list, cmd->args));
	else if (strcmp(cmd->args[0], "echo") == 0)
		return (builtin_echo(cmd->args, *env_list));
	else if (strcmp(cmd->args[0], "env") == 0)
		return (builtin_env(*env_list, cmd->args));
	else if (strcmp(cmd->args[0], "export") == 0)
		return (builtin_export(cmd->args, env_list));
	else if (strcmp(cmd->args[0], "pwd") == 0)
		return (builtin_pwd());
	else if (strcmp(cmd->args[0], "unset") == 0)
		return (builtin_unset(cmd->args, env_list));
	else if (strcmp(cmd->args[0], "exit") == 0)
		return (builtin_exit(cmd->args));
	return (CMD_NOT_FOUND);
}

int	execute_builtins(t_env **env_list, t_command *cmd)
{
	if (!cmd->args || !cmd->args[0])
		return (CMD_NOT_FOUND);
	handle_redirections(cmd->redirects, *env_list);
	return (execute_builtins_no_redir(env_list, cmd));
}
