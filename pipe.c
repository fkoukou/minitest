#include "minishell.h"


void redirections_pipe(int index,int nb_cmd,int pipefd[2])
{
    if (index == nb_cmd - 1)
    {
        dup2(pipefd[0],STDIN_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
    }
    else
    {
        dup2(pipefd[1],STDOUT_FILENO);
        close(pipefd[1]);
        close(pipefd[0]);
    }

}
int get_exit_status(int id)
{
    int res;
    waitpid(id,&res,0);
    if(WIFSIGNALED(res))
    {
        return 130;
    }
    return (WEXITSTATUS(res));
}
void test(char **args, t_env *env_list)
{
    char *cmd_path;

    cmd_path = find_in_path(args[0], env_list);
    if (!cmd_path)
    {
        printf("Command not found: %s\n", args[0]);
        return;
    }
        char **envp = env_list_to_array(env_list);
        execve(cmd_path, args, envp);
        perror("execve");
        exit(EXIT_FAILURE);
}

void pipeline(t_env **env_list, t_command *cmd_list, size_t nb_cmd)
{
    int pipefd[2];
    int prev_fd = -1;
    pid_t pid;
    size_t i = 0;

    while (cmd_list)
    {
        if (i < nb_cmd - 1 && pipe(pipefd) == -1)
        {
            perror("pipe");
            exit(1);
        }

        pid = fork();
        if (pid == 0)
        {
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            
            if (i < nb_cmd - 1)
            {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }

            if (execute_builtins(env_list, cmd_list) == CMD_NOT_FOUND)
                test(cmd_list->args, *env_list);
            exit(EXIT_SUCCESS);
        }
        else if (pid < 0)
        {
            perror("fork");
            exit(1);
        }

        if (prev_fd != -1)
            close(prev_fd);
        if (i < nb_cmd - 1)
        {
            close(pipefd[1]);
            prev_fd = pipefd[0];
        }

        cmd_list = cmd_list->next_pipe;
        i++;
    }

    for (size_t j = 0; j < nb_cmd; j++)
        wait(NULL);
}

