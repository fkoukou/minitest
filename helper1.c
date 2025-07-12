#include "minishell.h"

int     is_heredoc(t_token *head)
{
        if(!head)
                return (0);
        while (head->next)
                head = head->next;
        if (head->type == T_HEREDOC)
                return (1);
        return (0);
}
int ft_isalnum(int c)
{
    return ((c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9'));
}

int is_space(char c)
{
        if (c == ' ' || c == '\t' || c == '\n')
                return (1);
        return (0);
}


int     is_operator(char c)
{
        if (c == '>' || c == '<' || c == '|')
                return (1);
        return (0);
}