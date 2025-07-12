#include "minishell.h"

int     token_size(char *input)
{
        int     i;

        i = 0;
        if (is_operator(input[i]))
        {
                if ((input[i] == '<' && input[i + 1] == '<') ||
                        (input[i] == '>' && input[i + 1] == '>'))
                        return (2);
                return (1);
        }
        while (input[i] && !is_operator(input[i]) &&
                !is_space(input[i]))
        {
                if (input[i] == '\'' || input[i] == '"')
                {
                        i += check_quotes(input, i);
                        if (i == -1)
                                return (-1);
                }
                else
                        i++;
        }
        return (i);
}

char    *non_expand(char *input, int len)
{
        char    *token;
        char    c;
        int             i;
        int             j;

        i = 0;
        j = 0;
        token = malloc(len + 1);
        if (!token)
                return (NULL);
        while (i < len)
        {
                if (input[i] == '\'' || input[i] == '"')
                {
                        c = input[i++];
                        while (i < len && input[i] != c)
                                token[j++] = input[i++];
                        i++;
                }
                else
                        token[j++] = input[i++];
        }
        token[j] = '\0';
        return (token);
}

char *find_token(char *token, char *input, int len, t_env *env_list)
{
    int i = 0;
    int end;

    while (i < len)
    {
        if (input[i] == '\'')
        {
            end = check_quotes(input, i) + i;
            token = ft_strjoin(token, strnd(input, i + 1, (end - 1)));
            i = end;
        }
        else if (input[i] == '"')
        {
            i++;
            while (i < len && input[i] != '"')
                token = build_token_nq(&i, input, token, len, env_list);
            i++;
        }
        else
        {
            token = build_token_nq(&i, input, token, len, env_list);
        }
    }
    return token;
}


char *token(t_token *head, char *input, int len, t_type type, t_env *env_list)
{
    t_quote_type type_quote;
    char *token;

    type_quote = check_type_quotes(input, len, 0, 0);
    if (type != T_WORD || (type == T_WORD && type_quote == Q_SINGLE) || is_heredoc(head))
        return non_expand(input, len);

    token = strdup("");
    return find_token(token, input, len, env_list);  // Passer env_list à find_token
}

t_token *tokenize(char *input, t_env *env_list)
{
    t_token *head;
    int i;
    int len;
    char *value;
    int type_quote;

    i = 0;
    head = NULL;
    while (input[i])
    {
        while (is_space(input[i]))
            i++;
        if (!input[i])
            break;
        len = token_size(&input[i]);
        if (len == -1)
            return NULL;
        type_quote = is_q_or_non(&input[i], len);
        value = token(head, &input[i], len, opperator_type(&input[i]), env_list); // <-- ici on passe env_list
        add_token(&head, value, opperator_type(&input[i]), type_quote);
        i += len;
    }
    return head;
}
