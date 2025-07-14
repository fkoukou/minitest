#include "minishell.h"

int execute_builtins(t_env **env_list, t_command *cmd)
{
    if (!cmd->args || !cmd->args[0])
        return CMD_NOT_FOUND;
    while (cmd->redirects)
    {
        if (cmd->redirects->type == T_REDIR_IN)
            rediriger_entree(cmd->redirects->filename);
        else if (cmd->redirects->type == T_REDIR_OUT)
            rediriger_sortie(cmd->redirects->filename);
            else if (cmd->redirects->type == T_REDIR_APPEND)
                rediriger_sortie_append(cmd->redirects->filename);
            else if (cmd->redirects->type == T_HEREDOC)
                rediriger_heredoc(cmd->redirects->filename, *env_list);
            cmd->redirects = cmd->redirects->next;
        }
    if (strcmp(cmd->args[0], "cd") == 0)
        return builtin_cd(*env_list, cmd->args);        
    else if (strcmp(cmd->args[0], "echo") == 0)
        return builtin_echo(cmd->args, *env_list);
    else if (strcmp(cmd->args[0], "env") == 0)
        return builtin_env(*env_list, cmd->args);
    else if (strcmp(cmd->args[0], "export") == 0)
        return builtin_export(cmd->args, env_list);
    else if (strcmp(cmd->args[0], "pwd") == 0)
        return builtin_pwd();
    else if (strcmp(cmd->args[0], "unset") == 0)
        return builtin_unset(cmd->args, env_list);
    else if (strcmp(cmd->args[0], "exit") == 0)
        return builtin_exit(cmd->args);

    return CMD_NOT_FOUND;
}

