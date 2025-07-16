/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splitc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fakoukou <fakoukou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 06:25:20 by fakoukou          #+#    #+#             */
/*   Updated: 2025/07/16 08:01:38 by fakoukou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_words(const char *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i] && s[i] != c)
			count++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (count);
}

static char	*word_dup(const char *s, int start, int finish)
{
	char	*word;
	int		i;

	word = malloc(finish - start + 1);
	i = 0;
	while (start < finish)
		word[i++] = s[start++];
	word[i] = '\0';
	return (word);
}

static int	get_end(const char *s, int i, char c)
{
	if (s[i] == c)
		return (i);
	return (i + 1);
}

static void	fill_split(char **result, const char *s, char c)
{
	int	i;
	int	j;
	int	index;
	int	end;

	i = 0;
	j = 0;
	index = -1;
	while (s[i])
	{
		if (s[i] != c && index < 0)
			index = i;
		if ((s[i] == c || s[i + 1] == '\0') && index >= 0)
		{
			end = get_end(s, i, c);
			result[j++] = word_dup(s, index, end);
			index = -1;
		}
		i++;
	}
	result[j] = NULL;
}

char	**ft_split(char const *s, char c)
{
	char	**result;

	if (!s)
		return (NULL);
	result = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	fill_split(result, s, c);
	return (result);
}
