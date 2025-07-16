/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fakoukou <fakoukou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 06:32:35 by fakoukou          #+#    #+#             */
/*   Updated: 2025/07/16 08:54:03 by fakoukou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;
	char			x;
	size_t			i;

	ptr = b;
	i = 0;
	x = c;
	while (i < len)
	{
		ptr[i] = x;
		i++;
	}
	return (b);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
	{
		return (1);
	}
	return (0);
}

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*joined;
	int		len1;
	int		len2;

	len1 = strlen(s1);
	len2 = strlen(s2);
	joined = malloc(len1 + len2 + 1);
	if (!joined)
		return (NULL);
	strcpy(joined, s1);
	strcat(joined, s2);
	free(s1);
	return (joined);
}

size_t	size_cmd(t_command *cmd)
{
	size_t	i;

	i = 0;
	while (cmd)
	{
		i++;
		cmd = cmd->next_pipe;
	}
	return (i);
}
