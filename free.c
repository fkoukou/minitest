/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fakoukou <fakoukou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 06:24:28 by fakoukou          #+#    #+#             */
/*   Updated: 2025/07/16 06:24:56 by fakoukou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_arg(char **arg)
{
        int     i;

        i = 0;
        while (arg[i])
                free(arg[i++]);
        free(arg);
}

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head;
        head = head->next;
        if (tmp->value)
            free(tmp->value);
        free(tmp);
	}
}

void	free_redirects(t_redirect *redir)
{
	t_redirect	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->filename)
			free(redir->filename);
		free(redir);
		redir = tmp;
	}
}

void	free_command(t_command *cmd)
{
	t_command	*tmp;

	while (cmd)
	{
		tmp = cmd->next_pipe;
		if (cmd->args)
			free_arg(cmd->args);
		free_redirects(cmd->redirects);
		free(cmd);
		cmd = tmp;
	}
}
