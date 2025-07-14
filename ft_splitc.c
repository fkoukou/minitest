#include "minishell.h" 

static int count_words(const char *s, char c)
{
    int count = 0;
    int i = 0;

    while (s[i])
    {
        while (s[i] && s[i] == c)
            i++;
        if (s[i] && s[i] != c)
            count++;
        while (s[i] && s[i] != c)
            i++;
    }
    return count;
}

static char *word_dup(const char *s, int start, int finish)
{
    char *word = malloc(finish - start + 1);
    int i = 0;
    while (start < finish)
        word[i++] = s[start++];
    word[i] = '\0';
    return word;
}

char **ft_split(char const *s, char c)
{
    char **result;
    int i = 0, j = 0, index = -1;

    if (!s || !(result = malloc((count_words(s, c) + 1) * sizeof(char *))))
        return NULL;

    while (s[i])
    {
        if (s[i] != c && index < 0)
            index = i;
        else if ((s[i] == c || s[i + 1] == '\0') && index >= 0)
        {
            result[j++] = word_dup(s, index, (s[i] == c ? i : i + 1));
            index = -1;
        }
        i++;
    }
    result[j] = NULL;
    return result;
}