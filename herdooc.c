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
	// printf("%s\n", str);
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


// Fonction pour vérifier les guillemets dans le délimiteur
char *tmp_filename(void)
{
    char template[] = "/tmp/.heredoc_tmp_XXXXXX";
    int fd = mkstemp(template);
    if (fd == -1)
        return NULL;
    close(fd);
    return strdup(template);
}
void ft_putendl_fd(char *s, int fd)
{
    if (!s)
        return;
    write(fd, s, strlen(s));
    write(fd, "\n", 1);
}

// (Optionnel) Expansion des variables dans une ligne heredoc
char *expand_heredoc_line(char *line, t_env *env_list)
{
	(void)env_list; // Si vous n'utilisez pas env_list, vous pouvez l'ignorer
	// Implémentez votre propre fonction pour $VAR expansion
	return strdup(line); // remplacer par vraie expansion
}
char *expand_vars(char *line, t_env **env_list)
{
    (void)line;
    (void)env_list;
    // TODO : Implémenter l'expansion des variables $VAR ici
    return strdup(line);
}


char	*create_heredoc_file(char *raw_delimiter, t_env *env_list)
{
	int		quoted;
	int		expand;
	int		fd;
	char	*delim_clean;
	char	*line;
	char	*tmp_name;
	char	tmp_template[] = "/tmp/.heredoc_tmp_XXXXXX";

	quoted = is_quoted_type(raw_delimiter);
	expand = (quoted == 0);
	delim_clean = strdup(raw_delimiter);
	if (quoted)
		remove_surrounding_quotes(&delim_clean);

	tmp_name = strdup(tmp_template); // copy for mkstemp
	fd = mkstemp(tmp_name);
	if (fd == -1)
	{
		perror("mkstemp failed");
		free(delim_clean);
		free(tmp_name);
		return (NULL);
	}

	while (1)
	{
		line = readline("heredoc> ");
		if (!line || strcmp(line, delim_clean) == 0)
		{
			free(line);
			break;
		}
		char *expanded = expand ? expand_heredoc_line(line, env_list) : strdup(line);
		write(fd, expanded, strlen(expanded));
		write(fd, "\n", 1);
		free(line);
		free(expanded);
	}

	free(delim_clean);
	close(fd);
	return (tmp_name); // Return the name for later redirection
}




#include "minishell.h"

void	signal_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(1);
}

int	open_heredoc(t_redirect *redir, t_env **env)
{
	char	*line;
	int		fd;
	char	*filename;

	filename = tmp_filename();
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, redir->filename) == 0)
		{
			free(line);
			break;
		}
		if (redir->quote == 0)
			line = expand_vars(line, env);
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	fd = open(filename, O_RDONLY);
	unlink(filename);
	free(filename);
	return (fd);
}

int	handle_all_heredocs(t_redirect *redir, t_env **env_list)
{
	t_redirect	*current;
	int			fd;

	fd = -1;
	current = redir;
	while (current)
	{
if (current->type == T_HEREDOC)
		{
			if (fd != -1)
				close(fd);
			fd = open_heredoc(current, env_list);
		}
		current = current->next;
	}
	return (fd);
}

// void	register_heredoc(t_redirect *redir, t_env **env_list)
// {
// 	t_redirect	*tmp = redir;
// 	char		*last_file = NULL;
// 	char		*new_file;
// 	int			fd;

// 	while (tmp)
// 	{
// 		if (tmp->type == T_HEREDOC)
// 		{
// 			new_file = create_heredoc_file(tmp->filename, *env_list);
// 			if (last_file)
// 				unlink(last_file); // Supprime l'ancien fichier
// 			free(last_file);
// 			last_file = new_file; // Garde le dernier fichier
// 		}
// 		tmp = tmp->next;
// 	}
// 	if (last_file)
// 	{
// 		fd = open(last_file, O_RDONLY);
// 		if (fd != -1)
// 		{
// 			dup2(fd, STDIN_FILENO); // Redirige une seule fois
// 			close(fd);
// 		}
// 		unlink(last_file);
// 		free(last_file);
// 	}
// }
