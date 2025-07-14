#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int builtin_pwd(void)
{
    char *cwd;
    cwd = getcwd(NULL, 0);
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
