/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fakoukou <fakoukou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 06:20:38 by fakoukou          #+#    #+#             */
/*   Updated: 2025/07/16 07:07:45 by fakoukou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_env_aray(char **env_array, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

static char	**fill_env_array(t_env *env_list, char **env_array)
{
	t_env	*tmp;
	int		i;
	int		len;

	tmp = env_list;
	i = 0;
	while (tmp)
	{
		len = strlen(tmp->key) + strlen(tmp->value) + 2;
		env_array[i] = malloc(len);
		if (!env_array[i])
		{
			free_env_aray(env_array, i);
			return (NULL);
		}
		snprintf(env_array[i], len, "%s=%s", tmp->key, tmp->value);
		i++;
		tmp = tmp->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

char	**env_list_to_array(t_env *env_list)
{
	int		size;
	t_env	*tmp;
	char	**env_array;

	size = 0;
	tmp = env_list;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	env_array = malloc(sizeof(char *) * (size + 1));
	if (!env_array)
		return (NULL);
	return (fill_env_array(env_list, env_array));
}

char	*find_in_path(char *cmd, t_env *env_list)
{
	char	*full_path;
	char	**dirs;
	int		i;
	char	*path;

	i = 0;
	path = get_env_path(env_list);
	if (!path)
		return (NULL);
	dirs = ft_split(path, ':');
	while (dirs[i])
	{
		full_path = malloc(strlen(dirs[i]) + strlen(cmd) + 2);
		sprintf(full_path, "%s/%s", dirs[i], cmd);
		if (access(full_path, X_OK) == 0)
		{
			free_array(dirs);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(dirs);
	return (NULL);
}
// utils.c ou un autre fichier utile
int	ft_strisspace(const char *str)
{
	while (*str)
	{
		if (!(*str == ' ' || *str == '\t' || *str == '\n'))
			return (0);
		str++;
	}
	return (1);
}



// static void print_exec_error(char *cmd)
// {
//     if (access(cmd, F_OK) == 0)
//         perror("execve");
//     else
//         printf("Command not found: %s\n", cmd);
// }

static int is_path_cmd(char *cmd)
{
    return (cmd[0] == '/' || strncmp(cmd, "./", 2) == 0 || strncmp(cmd, "../", 3) == 0);
}

void free_str_array(char **array)
{
    int i = 0;
    if (!array)
        return;
    while (array[i])
        free(array[i++]);
    free(array);
}

void execute_external(char **args, t_env *env_list)
{
    pid_t pid;
    int status;
    char *cmd_path;
    char **envp;
    struct sigaction old_sigint, ignore_sig;

    if (!args || !args[0])
        return;

    if (is_path_cmd(args[0]))
        cmd_path = args[0];
    else
        cmd_path = find_in_path(args[0], env_list);

    if (!cmd_path)
    {
        printf("Command not found: %s\n", args[0]);
        return;
    }

    sigemptyset(&ignore_sig.sa_mask);
    ignore_sig.sa_handler = SIG_IGN;
    ignore_sig.sa_flags = 0;
    sigaction(SIGINT, &ignore_sig, &old_sigint);  // ignore SIGINT dans le parent

    envp = env_list_to_array(env_list);
    pid = fork();
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        execve(cmd_path, args, envp);
        perror("execve");
        free_str_array(envp);
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
    }
    else
        perror("fork");

    sigaction(SIGINT, &old_sigint, NULL); // restauration du handler SIGINT
    if (!is_path_cmd(args[0]))
        free(cmd_path);
    free_str_array(envp);

    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
        write(STDOUT_FILENO, "\n", 1);
}
