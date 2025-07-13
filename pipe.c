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
        // exit(130);
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
        // Convert env_list to array
        char **envp = env_list_to_array(env_list);
        execve(cmd_path, args, envp);
        perror("execve");
        exit(EXIT_FAILURE);
}

void pipeline(t_env **env_list, t_command *cmd_list,size_t nb_cmd)
{
    size_t i;
    int id;
    int pipefd[2];
    i = 0;
    while(i < nb_cmd)
    {
        pipe(pipefd);
        id = fork();
        if(id == 0)
        {
            redirections_pipe(i,nb_cmd,pipefd);
            if(execute_builtins(env_list, cmd_list) == CMD_NOT_FOUND)
                    test(cmd_list->args, *env_list);
        }
        else
        {
            dup2(pipefd[0],STDIN_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);
            i++;
            cmd_list = cmd_list->next_pipe;
        }
    }
    while(i--)
        wait(NULL);
    // printf("%d\n",get_exit_status(id));
}