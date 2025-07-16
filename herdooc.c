#include "minishell.h"
#include <ctype.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Récupère la valeur d’une variable d’environnement dans la liste
char	*get_env_value(t_env *env_list, const char *key)
{
	while (env_list)
	{
		if (strcmp(env_list->key, key) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

int	is_quoted_type(const char *str)
{
	size_t	len;

	len = strlen(str);
	printf("%s\n", str);
	if (len < 2)
		return (0);
	if (str[0] == '\'' && str[len - 1] == '\'')
		return (1);
	if (str[0] == '"' && str[len - 1] == '"')
		return (2);
	return (0);
}
void	remove_surrounding_quotes(char **str)
{
	size_t	len;
	char	*tmp;

	len = strlen(*str);
	if (len >= 2 &&
		((*str)[0] == '\'' || (*str)[0] == '"') &&
		(*str)[len - 1] == (*str)[0])
	{
		tmp = strndup(*str + 1, len - 2);
		free(*str);
		*str = tmp;
	}
}

char	*expand_heredoc_line(const char *line, t_env *env_list)
{
	int		len;
	char	*result;
	int		i;
	int		j;
	int		start;
	char	*varname;
	char	*val;

	len = strlen(line);
	result = malloc(len * 5 + 1);
	if (!result)
		return (NULL);
	i = 0, j = 0;
	while (i < len)
	{
		if (line[i] == '$' && i + 1 < len && (isalnum((unsigned char)line[i
					+ 1]) || line[i + 1] == '_'))
		{
			i++;
			start = i;
			while (i < len && (isalnum((unsigned char)line[i])
					|| line[i] == '_'))
				i++;
			varname = strndup(line + start, i - start);
			val = get_env_value(env_list, varname);
			if (val)
			{
				strcpy(result + j, val);
				j += strlen(val);
			}
			free(varname);
		}
		else
		{
			result[j++] = line[i++];
		}
	}
	result[j] = '\0';
	return (result);
}

t_quote_type1	check_type_quotes1(const char *input, int len)
{
	if (len >= 2 && input[0] == '\'' && input[len - 1] == '\'')
		return (SINGLE);
	if (len >= 2 && input[0] == '"' && input[len - 1] == '"')
		return (DOUBLE);
	return (NONE);
}

// Lit le contenu du heredoc
void	read_heredoc_content(char *delimiter, int expand, char *filename,
		t_env *env_list)
{
	int		fd;
	char	*line;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open heredoc file");
		return ;
	}
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			break ;
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		char *output_line = expand
			? expand_heredoc_line(line, env_list)
			: strdup(line); // Ne pas faire d'expansion
		write(fd, output_line, strlen(output_line));
		write(fd, "\n", 1);
		free(line);
		free(output_line);
	}
	close(fd);
}

void	rediriger_heredoc(char *raw_delimiter, t_env *env_list)
{
	static int	count;
	char		filename[64];
	char		*delim_raw;
	int			quoted_type;
	int			expand;
	char		*delim_clean;
	int			fd;

	count = 0;
	// 1. Dupliquer la string telle qu’elle a été tapée
	delim_raw = strdup(raw_delimiter);
	if (!delim_raw)
		return ;
	// 2. Détecter si quotes autour (sans encore les retirer !)
	quoted_type = is_quoted_type(delim_raw);
	// 3. Décider si expansion ou non
	expand = (quoted_type == 0);
	// 4. Créer une copie nettoyée des quotes seulement pour comparaison de fin
	delim_clean = strdup(delim_raw);
	if (quoted_type != 0)
		remove_surrounding_quotes(&delim_clean);
	// 5. Créer le fichier temporaire et y écrire le contenu
	snprintf(filename, sizeof(filename), ".heredoc_tmp_%d", count++);
	read_heredoc_content(delim_clean, expand, filename, env_list);
	// 6. Rediriger stdin vers ce fichier
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("open heredoc tmp file");
	}
	else
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	free(delim_raw);
	free(delim_clean);
}
