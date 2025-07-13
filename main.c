#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>



size_t size_cmd(t_command *cmd)
{
    size_t i;
    i = 0;
    while(cmd)
    {
        i++;
        cmd = cmd->next_pipe;
    }
    return i;
}
int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    int fd_in;
    int fd_out;

    t_env *env_list = create_env(envp);
    setup_signal_handlers();
    char *input = NULL;
    char cwd[PATH_MAX];
    char prompt[PATH_MAX + 3];
    t_token *tokens = NULL;
    t_command *cmd_list = NULL;
    // char *infile = NULL;
    // char *outfile = NULL;
    // int append = 0;

    while (1)
    {
        // Affichage du prompt dynamique
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            snprintf(prompt, sizeof(prompt), "%s$ ", cwd);
        else
            snprintf(prompt, sizeof(prompt), "$ ");

        input = readline(prompt);
        if (!input)
        {
            printf("exit\n");
            break;
        }

        if (input[0] == '\0')
        {
            free(input);
            continue;
        }

        add_history(input);

        // Tokenisation
        tokens = tokenize(input, env_list);
        if (!tokens)
        {
            free(input);
            continue;
        }
        // Parsing en structure de commandes
        cmd_list = parse_pipeline(&tokens);

        if (!cmd_list)
        {
            // free_tokens(tokens);
            free(input);
            continue;
        }
        // Exécution des commandes
            size_t nb_cmd =  size_cmd(cmd_list);
            if(nb_cmd == 1)
            {
                fd_in = dup(STDIN_FILENO);
                fd_out = dup(STDOUT_FILENO);
                if(execute_builtins(&env_list, cmd_list) == CMD_NOT_FOUND)
                    execute_external(cmd_list->args, env_list);
                dup2(fd_in,STDIN_FILENO);
                dup2(fd_out,STDOUT_FILENO);
            }
            else
            {
                pipeline(&env_list,cmd_list,nb_cmd);
                dup2(fd_in,STDIN_FILENO);
                dup2(fd_out,STDOUT_FILENO);
            }
        free(input);
    }
    free_env(env_list);
    return 0;
}
