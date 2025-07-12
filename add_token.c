#include "minishell.h"

t_type  opperator_type(char *input)
{
        if (input[0] == '|')
                return (T_PIPE);
        if (input[0] == '<' && input[1] == '<')

                return (T_HEREDOC);
        if (input[0] == '>' && input[1] == '>')
                return (T_REDIR_APPEND);
        if (input[0] == '<')
                return (T_REDIR_IN);
        if (input[0] == '>')
                return (T_REDIR_OUT);
        return (T_WORD);
}

t_token *new_token(char *value, int type, int quote)
{
        t_token *token;

        token = malloc(sizeof(t_token));
        if (!token)
                return (NULL);
        token->value = value;
        token->type = type;
        token->quote = quote;
        token->next = NULL;
        return (token);
}

void    add_token(t_token **head, char *value, int type, int quote)
{
        t_token *new;
        t_token *mov;

        new = new_token(value, type, quote);
        if (!new)
                return ;
        if (!*head)
                *head =  new;
        else
        {
                mov = *head;
                while (mov->next)
                        mov = mov->next;
                mov->next = new;
        }
}