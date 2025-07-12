#include "minishell.h"
void free_tokens(t_token *head) {
    t_token *tmp;
    while (head) {
        tmp = head->next;
        free(head->value);
        free(head);
        head = tmp;
    }
}

void free_redirects(t_redirect *redir) {
    t_redirect *tmp;
    while (redir) {
        tmp = redir->next;
        free(redir->filename);
        free(redir);
        redir = tmp;
    }
}

void free_command(t_command *cmd) {
    t_command *tmp;
    int i;

    while (cmd) {
        tmp = cmd->next_pipe;
        if (cmd->args) {
            i = 0;
            while (cmd->args[i])
                free(cmd->args[i++]);
            free(cmd->args);
        }
        free_redirects(cmd->redirects);
        free(cmd);
        cmd = tmp;
    }
}