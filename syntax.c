#include "minishell.h"

t_redirect	*new_redirect(int type, char *file, int quote, t_env **env_list)
{
	t_redirect	*new_node;
	(void)env_list; // env_list not used in this function, but kept for consistency
	new_node = malloc(sizeof(t_redirect));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->filename = file;
	new_node->quote = quote;
	new_node->next = NULL;
	return (new_node);
}

t_redirect	*add_redirect(t_redirect **red, t_redirect *new_red)
{
	t_redirect	*mov;

	if (!*red)
		*red = new_red;
	else
	{
		mov = *red;
		while (mov->next)
			mov = mov->next;
		mov->next = new_red;
	}
	return (*red);
}

void	free_arg(char **arg)
{
	int	i;

	i = 0;
	while (arg[i])
		free(arg[i++]);
	free(arg);
}

char	**add_arg(char *value, char **arg)
{
	int		i;
	int		j;
	char	**new_arg;

	i = 0;
	j = 0;
	while (arg && arg[i])
		i++;
	new_arg = malloc((i + 2) * sizeof(char *));
	if (!new_arg)
		return (NULL);
	while (j < i)
	{
		new_arg[j] = arg[j];
		j++;
	}
	new_arg[j++] = value;
	new_arg[j] = NULL;
	return (new_arg);
}

t_command	*parse_command(t_token **tokens, t_env **env_list)
{
	t_command	*cmd;
	int			type;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->next_pipe = NULL;
	while (*tokens && (*tokens)->type != T_PIPE)
	{
		if ((*tokens)->type == T_REDIR_OUT || (*tokens)->type == T_REDIR_IN ||
			(*tokens)->type == T_REDIR_APPEND || (*tokens)->type == T_HEREDOC)
		{
			type = (*tokens)->type;
			*tokens = (*tokens)->next;
			if (!*tokens || (*tokens)->type != T_WORD)
				return (NULL);
			cmd->redirects = add_redirect(&cmd->redirects, new_redirect(type,
						(*tokens)->value, (*tokens)->quote,env_list));
		}
		else if ((*tokens)->type == T_WORD)
			cmd->args = add_arg((*tokens)->value, cmd->args);
		if (*tokens)
			*tokens = (*tokens)->next;
	}
	return (cmd);
}

t_command	*parse_pipeline(t_token **tokens, t_env **env_list)
{
	t_command *cmd;
	t_command *head;
	t_command *mov;

	head = NULL;
	if ((*tokens)->type == T_PIPE)
		return (NULL); //free
	while (*tokens)
	{
		cmd = parse_command(tokens,env_list);
		if (!cmd)
			return (NULL); //free
		if (!head)
			head = cmd;
		else
		{
			mov = head;
			while (mov->next_pipe)
				mov = mov->next_pipe;
			mov->next_pipe = cmd;
		}
		if (!(*tokens))
			break ;
		if ((*tokens)->type == T_PIPE && ((!(*tokens)->next)
				|| ((*tokens)->next->type == T_PIPE)))
			return (NULL);
		else if ((*tokens)->type == T_PIPE || *tokens)
			*tokens = (*tokens)->next;
	}
	return (head);
}