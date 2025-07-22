#include "minishell.h"

int	token_size(char *input)
{
	int	i;
	int	r;

	i = 0;
	if (is_operator(input[i]))
	{
		if ((input[i] == '<' && input[i + 1] == '<') ||
			(input[i] == '>' && input[i + 1] == '>'))
			return (2);
		return (1);
	}
	while (input[i] && !is_operator(input[i]) &&
			!is_space(input[i]))
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			r = check_quotes(input, i);
			if (r == -1)
				return (-1);
			i += r;
		}
		else
			i++;
	}
	return (i);
}

char	*non_expand(char *input, int len, int heredoc)
{
	char	*token;
	char	c;
	int		i;
	int		j;

	i = 0;
	j = 0;
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	while (i < len)
	{
		if ((input[i] == '\'' || input[i] == '"') && !heredoc)
		{
			c = input[i++];
			while (i < len && input[i] != c)
				token[j++] = input[i++];
			i++;
		}
		else
			token[j++] = input[i++];
	}
	token[j] = '\0';
	return (token);
}

int     is_expand(int *i, char *input)
{
        if (input[*i] == '$' && input[(*i) + 1] != '$' &&
                (ft_isalnum(input[(*i) + 1]) || input[(*i) + 1] == '_'))
                return (1);
        return (0);
}

char	*build_token_sq(char *input, char *token, int *i)
{
	int	end;
	end = check_quotes(input, *i) + *i;
        token = ft_strjoin(token, strnd(input, *i + 1, (end - 1)));//*add free
        *i = end;
	return(token);
}
char	*find_token(t_token **head, char *input, int len, t_env *env_list)
{
	int	i;
	char	*token;

	token = strdup("");
	i = 0;
	while (i < len)
	{
		if (input[i] == '\'')
			token = build_token_sq(input, token, &i);
		else if (input[i] == '"')
		{
			i++;
			while (i < len && input[i] != '"')
				token = build_token_dq(&i, input, token, env_list);
			i++;
		}
		else
		{
			if (is_expand(&i, input))
				token = build_token_expand_nq(head, expansion(&i, len, input, env_list), token);
			else
				token = build_token_nq(input, token, &i, len);
		}
	}
	return (token);
}

char	*token(t_token **head, char *input, int len, t_env *env_list)
{
	t_quote_type	type_quote;
	t_type	type;

	type = opperator_type(input);
	type_quote = check_type_quotes(input, len, 0, 0);
	if (type == T_WORD && is_heredoc(*head))
                return (non_expand(input, len, 1));
	else if (type != T_WORD || (type == T_WORD && type_quote == Q_SINGLE))
		return (non_expand(input, len, 0));
	return (find_token(head, input, len, env_list));
}

t_token	*tokenize(char *input, t_env *env_list)
{
	t_token	*head;
	int		i;
	int		len;
	char	*value;

	i = 0;
	head = NULL;
	while (input[i])
	{
		while (is_space(input[i]))
			i++;
		if (!input[i])
			break ;
		len = token_size(&input[i]);
		if (len == -1)
			return (NULL);
		value = token(&head, &input[i], len, env_list);
		if (value[0])
			add_token(&head, value, opperator_type(&input[i]), is_q_or_non(&input[i], len));
		else
			free (value);
		i += len;
	}
	return (head);
}
