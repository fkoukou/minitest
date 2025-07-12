#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int builtin_pwd(char *cached_pwd)
{
    char buf[PATH_MAX];
    char *cwd;

    if (cached_pwd && cached_pwd[0] != '\0')
    {
        printf("%s\n", cached_pwd);
        return 0;
    }

    cwd = getcwd(buf, sizeof(buf));
    if (cwd)
    {
        printf("%s\n", cwd);
        return 0;
    }
    else
    {
        perror("pwd");
        return 1;
    }
}
