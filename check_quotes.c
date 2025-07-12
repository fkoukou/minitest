#include "minishell.h"

int     check_quotes(char *input, int i)
{
        char    quote;
        int             count;

        count = 1;
        quote = input[i++];
        while (input[i] && input[i] != quote)
        {
                count++;
                i++;
        }
        if (input[i] == quote)
                return (count + 1);
        return (-1);
}

t_quote_type    ft_next_tq(int single_q, int double_q, int none_q)
{
        if (!double_q && !single_q && none_q)
                return (Q_NONE);// --> expand
        if (single_q && !double_q && !none_q)//'$user' mime len
                return (Q_SINGLE);// --> non expand
        if (!single_q && double_q && !none_q )// "$user" virifer
                return (Q_DOUBLE);// --> expand
        //!single_q && !double_q && !none_q imposible tkon
        //!singlce_q && double_q && none_q// "$user"$user --> expand
        //single_q && !double_q && none_q// '$user'$user --> expand
        //single_q && double_q && none_q// "$user"'$user'$user --> expand
        //single_q && double_q && !none_q// "$user"'$user' --> expand
        return (Q_DANDS);
}

t_quote_type    check_type_quotes(char *input, int len, int single_q, int double_q)
{
        int     i;
        int     none_q;

        i = 0;
        none_q = 0;
        while (i < len)
        {
                if (input[i] == '"')
                {
                        double_q = 1;
                        i += check_quotes(input, i);
                }
                else if (input[i] == '\'')
                {
                        single_q = 1;
                        i += check_quotes(input, i);
                }
                else if (i < len)
                {
                        none_q = 1;
                        i++;
                }
        }
        return (ft_next_tq(single_q, double_q, none_q));
}

int is_q_or_non(char *input, int len)
{
        t_quote_type    type_quote;

        type_quote = check_type_quotes(input, len, 0, 0);
        if (type_quote == Q_NONE)
                return (0);
        return (1);
}


