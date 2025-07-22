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


char    *expansion(int *i, int len, char *input, t_env *env_list)
{
        int             start;
        char *var;
        char    *expan;

        start = *i;
        (*i)++;
        while (*i < len && (ft_isalnum(input[*i]) ||
                input[*i] == '_'))
                (*i)++;
        var = add_expansion(start, *i, input);
        expan = get_env_value(env_list, var);
        free(var);
        if (!expan)
                return (ft_strdup(""));
        return (ft_strdup(expan));
}

int check_expand(char *expand)
{
        int i;

        i = 0;
        while (expand[i])
        {
                if (ft_iswhitespaces(expand[i]))
                        return (1);
                i++;
        }
        return (0);
}
char *check_add_expand(t_token **head, char **value, char *token, char *expand)
{
	char *ret;
	int i;

	i = 0;
    if (!ft_iswhitespaces(expand[0]) && value[i])
			token = ft_strjoin(token, value[i++]);
	if (token[0])
    		add_token(head, token, T_WORD, 0);
	else
		free(token);
    while (value[i])
    {
        if (!value[i + 1])
            break;
        add_token(head, value[i], T_WORD, 0);
        i++;
    }
    if (!ft_iswhitespaces(expand[ft_strlen(expand) - 1]) && value[i])
        ret = ft_strdup(value[i]);
    else if (value[i])
	{
        add_token(head, value[i], T_WORD, 0);
		ret = ft_strdup("");
	}
	return (ret);
}
char *add_all_expand(t_token **head, char *expand, char *token)
{
    char **value;
	char *ret;

    if (!expand || !*expand)
    {
        add_token(head, token, T_WORD, 0);
        return (ft_strdup(""));
    }
    value = ft_split_whitespaces(expand);
    if (!value)
		return (token);
	ret = check_add_expand(head, value, token, expand);
	free_arg(value);
    return (ret);
}

char    *build_token_expand_nq(t_token **head, char *expanded, char *token)
{
        if (check_expand(expanded))
                token = add_all_expand(head, expanded, token);
        else
                token = ft_strjoin(token, expanded);
        return (token);
}

char    *build_token_nq(char *input, char *token, int *i, int len)
{
        int start;
        int t;

        t = 0;
        if (input[*i] == '$' && input[(*i) + 1] == '$')
                (*i) += 2;
        else if (input[*i] == '$')
        {
                t = 1;
                start = *i;
                (*i)++;
        }
        if (t == 0)
                start = *i;
        while (*i < len && input[*i] != '$' && input[*i] != '"' && input[*i] != '\'')
                (*i)++;
        if (start != *i)
                token = ft_strjoin(token, strnd(input, start, *i));
        return (token);
}

char    *build_token_dq(int *i, char *input, char *token, t_env *env_list)
{
        int start;
        int t;

        t = 0;
        if (is_expand(i, input))
                token = ft_strjoin(token, expansion(i, token_size(input), input, env_list));
        else
        {
                if (input[*i] == '$' && input[(*i) + 1] == '$')
                        (*i) += 2;
                else if (input[*i] == '$')
                {
                        t = 1;
                        start = *i;
                        (*i)++;
                }
                if (t == 0)
                        start = *i;
                while (*i < token_size(input) && input[*i] != '$' && input[*i] != '"')
                        (*i)++;
                if (start != *i)
                        token = ft_strjoin(token, strnd(input, start, *i));
        }
        return (token);
}

