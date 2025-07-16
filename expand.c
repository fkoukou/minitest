/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fakoukou <fakoukou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 06:21:38 by fakoukou          #+#    #+#             */
/*   Updated: 2025/07/16 06:21:46 by fakoukou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*add_expansion(int start, int end, char *input)
{
	char	*var;
	int		k;
	int		i;

	k = end - start - 1;
	i = 0;
	var = malloc(k + 1);
	if (!var)
		return (NULL);
	while (i < k)
		var[i++] = input[++start];
	var[i] = '\0';
	return (var);
}

char	*expansion(int *i, int len, char *input, t_env *env_list)
{
	int		start;
	char	*var;
	char	*expan;

	start = *i;
	(*i)++;
	while (*i < len && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	var = add_expansion(start, *i, input);
	expan = get_env_value(env_list, var);
	free(var);
	if (!expan)
		return (ft_strdup(""));
	return (ft_strdup(expan));
}

char	*build_token_nq(int *i, char *input, char *token, int len,
		t_env *env_list)
{
	int	start;

	if (input[*i] == '$' && input[(*i) + 1] != '$' && input[(*i) + 1] &&
		(ft_isalnum(input[(*i) + 1]) || input[(*i) + 1] == '_'))
		token = ft_strjoin(token, expansion(i, len, input, env_list));
	else
	{
		start = *i;
		if (input[*i] == '$' && input[(*i) + 1] == '$')
			(*i) += 2;
		else if (input[*i] == '$')
			(*i)++;
		while (*i < len && input[*i] != '$' && input[*i] != '"'
			&& input[*i] != '\'')
			(*i)++;
		token = ft_strjoin(token, strnd(input, start, *i));
	}
	return (token);
}

char	*build_token_dq(int *i, char *input, char *token, int len,
		t_env *env_list)
{
	int	start;

	if (input[*i] == '$' && input[(*i) + 1] != '$' && input[(*i) + 1] &&
		(ft_isalnum(input[(*i) + 1]) || input[(*i) + 1] == '_'))
		token = ft_strjoin(token, expansion(i, len, input, env_list));
	else
	{
		start = *i;
		if (input[*i] == '$' && input[(*i) + 1] == '$')
			(*i) += 2;
		else if (input[*i] == '$')
			(*i)++;
		while (*i < len && input[*i] != '$' && input[*i] != '"')
			(*i)++;
		token = ft_strjoin(token, strnd(input, start, *i));
	}
	return (token);
}
