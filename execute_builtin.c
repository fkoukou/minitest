#include <string.h> // pour strcmp
#include "minishell.h"

int execute_builtins(t_env *env_list, char **args, char *cached_pwd)
{
    if (!args || !args[0])
        return CMD_NOT_FOUND;

    if (strcmp(args[0], "cd") == 0)
        return builtin_cd(env_list, args);
    else if (strcmp(args[0], "echo") == 0)
        return builtin_echo(args);
    else if (strcmp(args[0], "env") == 0)
        return builtin_env(env_list, args);
    else if (strcmp(args[0], "export") == 0)
        return builtin_export(args, &env_list);
    else if (strcmp(args[0], "pwd") == 0)
        return builtin_pwd(cached_pwd);
    else if (strcmp(args[0], "unset") == 0)
        return builtin_unset(args, &env_list);
    else if (strcmp(args[0], "exit") == 0)
        return builtin_exit(args);

    return CMD_NOT_FOUND;
}
