#include "minishell.h"

char    *ft_substr(char *s, int start, int len)
{
        char    *ss;
        size_t  i;
        size_t  siz;

        if (!s)
                return (NULL);
        if (start >= ft_strlen(s))
                return (ft_strdup(""));
        if (ft_strlen(s) - start < len)
                siz = ft_strlen(s) - start;
        else
                siz = len;
        ss = malloc((siz + 1) * sizeof(char));
        if (!ss)
                return (NULL);
        i = 0;
        while (i < siz && s[i])
        {
                ss[i] = s[start + i];
                i++;
        }
        ss[i] = '\0';
        return (ss);
}

static int      ft_nworth(char *s)
{
        int     i;
        int     count;
        int     in_word;

    i = 0;
        count = 0;
        in_word = 0;
        if (s[0] == '\0')
                return (1);
    while (s[i])
    {
        if (ft_iswhitespaces(s[i]))
            in_word = 0;
        else if (!in_word)
        {
            in_word = 1;
            count++;
        }
        i++;
    }
    return (count + 1);

}

void    *ft_free(char **a, int j)
{
        while (j > 0)
        {
                free(a[j - 1]);
                j--;
        }
        free(a);
        return (NULL);
}

static char     **ft_fill(char *s, char **a)
{
        size_t  i;
        size_t  j;
        size_t  start;
        size_t  fin;

        start = 0;
        fin = 0;
        i = 0;
        j = 0;
        while (s[i])
        {
                if (ft_iswhitespaces(s[i]) && !ft_iswhitespaces(s[i + 1]) && s[i + 1] != '\0')
                        start = i + 1;
                if ((!ft_iswhitespaces(s[i]) && ft_iswhitespaces(s[i + 1])) || (!ft_iswhitespaces(s[i]) && s[i + 1] == '\0'))
                {
                        fin = i + 1;
                        a[j] = ft_substr(s, start, fin - start);
                        if (!a[j])
                                return (ft_free(a, j));
                        j++;
                }
                i++;
        }
        a[j] = NULL;
        return (a);
}

char    **ft_split_whitespaces(char *s)
{
        char    **a;

        if (!s)
                return (NULL);
        a = malloc(ft_nworth(s) * sizeof(char *));
        if (!a)
                return (NULL);
        return (ft_fill(s, a));
}
