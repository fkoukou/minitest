#include "minishell.h"
void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;
	char			x;
	size_t			i;

	ptr = b;
	i = 0;
	x = c;
	while (i < len)
	{
		ptr[i] = x;
		i++;
	}
	return (b);
}


int ft_strcmp(const char *s1, const char *s2)
{
    size_t i = 0;

    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
	{
		return (1);
	}
	return (0);
}
