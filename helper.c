#include "minishell.h"

int ft_strlen(char *s1)
{
    int i = 0;
    while (s1[i])
        i++;
    return (i);
}

char *ft_strdup(char *s1)
{
        char    *s;
        int             i;

        s = malloc((ft_strlen(s1) + 1) * sizeof(char));
        if (!s)
                return (NULL);
        i = 0;
        while (s1[i])
        {
                s[i] = s1[i];
                i++;
        }
        s[i] = '\0';
        return (s);
}

char *ft_strjoin(char *s1, char *s2)
{
    char *s3;
    int tl, i, j;

    if (!s1 && !s2)
        return NULL;
    if (!s1)
        return strdup(s2);
    if (!s2)
        return strdup(s1);

    tl = ft_strlen(s1) + ft_strlen(s2);
    s3 = malloc(sizeof(char) * (tl + 1));
    if (!s3)
        return NULL;

    i = 0;
    while (s1[i])
    {
        s3[i] = s1[i];
        i++;
    }
    j = 0;
    while (s2[j])
        s3[i++] = s2[j++];
    s3[i] = '\0';

    return s3;
}



char *strnd(char *s1, int start, int end)
{
	char	*s;
	int		i;
        int     len;

        len = end - start;
	s = malloc((len + 1)* sizeof(char));
	if (!s)
		return (NULL);
	i = 0;
	while ( i < len)
		s[i++] = s1[start++];
	s[i] = '\0';
	return (s);
}