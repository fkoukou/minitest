#include "minishell.h"


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
char *expand_heredoc_line(const char *line, t_env *env_list)
{
    int len = strlen(line);
    char *result = malloc(len * 5 + 1);
    if (!result)
        return NULL;
    int i = 0, j = 0;
    while (i < len)
    {
        if (line[i] == '$' && i + 1 < len && (isalnum((unsigned char)line[i+1]) || line[i+1] == '_'))
        {
            i++;
            int start = i;
            while (i < len && (isalnum((unsigned char)line[i]) || line[i] == '_'))
                i++;
            char *varname = strndup(line + start, i - start);
            char *val = get_env_value(env_list, varname);
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
    return result;
}

char *tmp_filename(void)
{
char template[] = "/tmp/heredoc_tmp_XXXXXX";
    int fd = mkstemp(template);
    if (fd == -1)
        return NULL;
    close(fd);
    return strdup(template);
}

#include <signal.h>


void get_read_herdooc(t_redirect *red, int fd, t_env *env_list)
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
		char *expanded = NULL;

if (!red->quote && strchr(line, '$'))
	expanded = expand_heredoc_line(line, env_list);
else
	expanded = strdup(line);

write(fd, expanded, strlen(expanded));
write(fd, "\n", 1);
free(expanded);

	}
}


int	open_herdoc(t_redirect *red, t_env *env_list)
{
	int		fd;
	char	*filename;
	int		id;
	int		status;

	signal(SIGINT, SIG_IGN); 

	filename = tmp_filename(); 
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	red->quote = is_quoted_type(red->filename);
	if (red->quote)
	remove_surrounding_quotes(&(red->filename));

	id = fork();
	if (id < 0)
		return (-1);
	if (id == 0)
	{
		// Processus enfant : lit les lignes de l'utilisateur
		signal(SIGINT, sigint_handler);
		get_read_herdooc(red,fd, env_list);  // écrire le contenu
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
// cleanup_all_heredocs(cmd->redirects);
	return (0);
}
