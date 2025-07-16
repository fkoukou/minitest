/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fakoukou <fakoukou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 06:30:25 by fakoukou          #+#    #+#             */
/*   Updated: 2025/07/16 06:30:51 by fakoukou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>

void	rediriger_entree(const char *fichier)
{
	int	fd;

	fd = open(fichier, O_RDONLY);
	if (fd < 0)
	{
		perror("open infile");
		return ;
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	rediriger_sortie(const char *fichier)
{
	int	fd;

	fd = open(fichier, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open outfile");
		return ;
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	rediriger_sortie_append(const char *fichier)
{
	int	fd;

	fd = open(fichier, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("open append");
		return ;
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}
