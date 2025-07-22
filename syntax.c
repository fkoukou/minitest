#include "minishell.h"

t_redirect	*new_redirect(int type, char *file, int quote, t_env **env_list)
{
	t_redirect	*new_node;
	
	new_node = malloc(sizeof(t_redirect));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->filename = file;
	// printf("%d", new_node->type);
	if(new_node->type == 5)
	{
		open_herdoc(new_node, *env_list); 
	}
	new_node->quote = quote;
	new_node->next = NULL;
	return (new_node);
}

t_redirect	*add_redirect(t_redirect **red, t_redirect *new_red)
{
	t_redirect	*mov;

	if (!new_red)
		return (NULL);
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
	if (arg)
		free_arg(arg);
	return (new_arg);
}

void	add_cmd(t_command **cmd)
{
	*cmd = malloc(sizeof(t_command));
	if (!(*cmd))
		return ;
	(*cmd)->args = NULL;
	(*cmd)->redirects = NULL;
	(*cmd)->next_pipe = NULL;
}

t_command	*parse_command(t_token **tokens, t_env **env_list)
{
	t_command	*cmd;
	int			type;

	add_cmd(&cmd);
	if (!cmd)
		return (NULL);
	while (*tokens && (*tokens)->type != T_PIPE)
	{
		if ((*tokens)->type == T_REDIR_OUT || (*tokens)->type == T_REDIR_IN ||
			(*tokens)->type == T_REDIR_APPEND || (*tokens)->type == T_HEREDOC)
		{
			type = (*tokens)->type;
			*tokens = (*tokens)->next;
			if (!*tokens || (*tokens)->type != T_WORD)
				return (write(2, "syntax error: redirection operator used without a file\n", 55), NULL);
			cmd->redirects = add_redirect(&cmd->redirects, new_redirect(type, (*tokens)->value, (*tokens)->quote, env_list));
			if (!cmd->redirects)
				return (NULL);
		}
		else if ((*tokens)->type == T_WORD)
			cmd->args = add_arg((*tokens)->value, cmd->args);
		if (*tokens)
			*tokens = (*tokens)->next;
	}
	return (cmd);
}

void	add_next_pipe(t_command **head, t_command *cmd)
{
	t_command *mov;

	if (!*head)
		*head = cmd;
	else
	{
		mov = *head;
		while (mov->next_pipe)
			mov = mov->next_pipe;
		mov->next_pipe = cmd;
	}
}

t_command	*parse_pipeline(t_token **tokens, t_env **env_list)
{
	t_command *cmd;
	t_command *head;

	head = NULL;
	if (*tokens && (*tokens)->type == T_PIPE)
		return (write(2, "syntax error near unexpected token `|'\n", 39), NULL); //free
	while (*tokens)
	{
		cmd = parse_command(tokens,env_list);
		if (!cmd)
			return (NULL); //free
		add_next_pipe(&head, cmd);
		if (!(*tokens))
			break ;
		if ((*tokens)->type == T_PIPE && ((!(*tokens)->next)
				|| ((*tokens)->next->type == T_PIPE)))
			return (write(2, "syntax error near unexpected token `|'\n", 39), NULL);
		else if ((*tokens)->type == T_PIPE || *tokens)
			*tokens = (*tokens)->next;
	}
	return (head);
}