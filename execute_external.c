#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minishell.h"
char **env_list_to_array(t_env *env_list)
{
    int size = 0;
    t_env *tmp = env_list;
    while (tmp)
    {
        size++;
        tmp = tmp->next;
    }

    char **env_array = malloc(sizeof(char *) * (size + 1));
    if (!env_array)
        return NULL;

    tmp = env_list;
    int i = 0;
    while (tmp)
    {
        int len = strlen(tmp->key) + strlen(tmp->value) + 2;
        env_array[i] = malloc(len);
        if (!env_array[i])
        {
            for (int j = 0; j < i; j++)
                free(env_array[j]);
            free(env_array);
            return NULL;
        }
        snprintf(env_array[i], len, "%s=%s", tmp->key, tmp->value);
        i++;
        tmp = tmp->next;
    }
    env_array[i] = NULL;
    return env_array;
}

char *get_env_path(t_env *env_list)
{
    while (env_list)
    {
        if (strcmp(env_list->key, "PATH") == 0)
            return env_list->value;
        env_list = env_list->next;
    }
    return NULL;
}

char *find_in_path(char *cmd, t_env *env_list)
{
    char *full_path;
    char **dirs;
    int i = 0;

    char *path = get_env_path(env_list);
    if (!path)
        return NULL;

    dirs = ft_split(path, ':');
    while (dirs[i])
    {
        full_path = malloc(strlen(dirs[i]) + strlen(cmd) + 2);
        sprintf(full_path, "%s/%s", dirs[i], cmd);
        if (access(full_path, X_OK) == 0)
        {
            free_array(dirs);
            return full_path;
        }
        free(full_path);
        i++;
    }
    free_array(dirs);
    return NULL;
}

void execute_external(char **args, t_env *env_list)
{
    pid_t pid;
    int status;
    char *cmd_path;

    cmd_path = find_in_path(args[0], env_list);
    if (!cmd_path)
    {
        printf("Command not found: %s\n", args[0]);
        return;
    }
    pid = fork();
    if (pid == 0)
    {
        char **envp = env_list_to_array(env_list);
        execve(cmd_path, args, envp);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else
        waitpid(pid, &status, 0);
    free(cmd_path);
}
