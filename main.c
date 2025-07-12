#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    t_env *env_list;
    char *input;
    char **args;
    int ret;
    char cwd[PATH_MAX];

    (void)argc;
    (void)argv;

    env_list = create_env(envp);

    while (1)
    {
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s$ ", cwd);
        else
            printf("$ ");

        input = readline(NULL);
        if (!input) // Ctrl+D
            break;

        if (input[0] == '\0')
        {
            free(input);
            continue;
        }

        add_history(input);

        // Utilise tokenize_echo_args pour respecter les guillemets et garder les espaces
        args = tokenize_echo_args(input);

        if (args && args[0])
        {
            ret = execute_builtins(env_list, args, cwd);

            if (ret == CMD_NOT_FOUND)
            {
                execute_external(args, env_list);
            }
        }

        free_array(args);
        free(input);
    }

    free_env(env_list);

    return 0;
}

