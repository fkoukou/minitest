#include "minishell.h"


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


char *tmp_filename(void)
{
    char template[] = "heredoc_tmp_XXXXXX";
    int fd = mkstemp(template);
    if (fd == -1)
        return NULL;
    close(fd);
    return strdup(template);
}

#include <signal.h>


void	get_read_herdooc(t_redirect *red, int fd)
{
	char	*line = NULL;
	// size_t	len = 0;

	while (1)
	{
		line = readline(">");
		if(!line)
		{
			printf("warning: here-document terminated unexpectedly (EOF)\n");
			return ;		
		}
		if (strcmp(line, red->filename) == 0)
			break;
		// si expansion souhaitée: line = expand_variables(line, env_list);
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

int	open_herdoc(t_redirect *red)
{
	int		fd;
	char	*filename;
	int		id;
	int		status;

	signal(SIGINT, SIG_IGN); // ignorer Ctrl+C dans le shell parent

	filename = tmp_filename(); // génère nom fichier temporaire
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);

	id = fork();
	if (id < 0)
		return (-1);
	if (id == 0)
	{
		// Processus enfant : lit les lignes de l'utilisateur
		signal(SIGINT, sigint_handler);
		get_read_herdooc(red,fd);  // écrire le contenu
		close(fd);
		// red->file = filename;
		// free(filename);
		exit(0);
	}
	else
	{
		// Processus parent : attendre que l'enfant termine
		waitpid(id, &status, 0);
		close(fd);
	}
	// sauvegarder filename dans redirection pour le rouvrir après
	red->filename = filename;
	return (0);
}
