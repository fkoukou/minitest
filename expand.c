#include "minishell.h"

char *add_expansion(int start, int end, char *input)
{
    char *var;
    int k = end - start - 1;
    int i = 0;

    var = malloc(k + 1);
    if (!var)
        return NULL;
    while (i < k)
        var[i++] = input[++start];
    var[i] = '\0';
    return var;
}

char *expansion(int *i, int len, char *input, t_env *env_list)
{
    int start = *i;
    (*i)++;
    while (*i < len && (ft_isalnum(input[*i]) || input[*i] == '_'))
        (*i)++;
    char *var = add_expansion(start, *i, input);
    char *expan = get_env_value(env_list, var);
    free(var);
    if (!expan)
        return ft_strdup("");
    return ft_strdup(expan);
}



char *build_token_nq(int *i, char *input, char *token, int len, t_env *env_list)
{
        int start;

       if (input[*i] == '$' && input[(*i) + 1] != '$' && input[(*i) + 1] &&
    (ft_isalnum(input[(*i) + 1]) || input[(*i) + 1] == '_'))
    token = ft_strjoin(token, expansion(i, len, input, env_list));

        else
        {
                start = *i;
                if (input[*i] == '$' && input[(*i) + 1] == '$')
                        (*i) += 2;
                else if (input[*i] == '$')
                        (*i)++;
                while (*i < len && input[*i] != '$' && input[*i] != '"' && input[*i] != '\'')
                        (*i)++;
               
                token = ft_strjoin(token, strnd(input, start, *i));
        }
        return (token);
}

char *build_token_dq(int *i, char *input, char *token, int len, t_env *env_list)
{
    int start;

    if (input[*i] == '$' && input[(*i) + 1] != '$' && input[(*i) + 1] &&
        (ft_isalnum(input[(*i) + 1]) || input[(*i) + 1] == '_'))
        token = ft_strjoin(token, expansion(i, len, input, env_list));  // <-- ici on passe env_list
    else
    {
        start = *i;
        if (input[*i] == '$' && input[(*i) + 1] == '$')
            (*i) += 2;
        else if (input[*i] == '$')
            (*i)++;
        while (*i < len && input[*i] != '$' && input[*i] != '"')
            (*i)++;
        token = ft_strjoin(token, strnd(input, start, *i));
    }
    return (token);
}
