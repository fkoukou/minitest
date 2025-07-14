
#include "minishell.h"

static char *get_env_value1(t_env *env, const char *key)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
            return env->value;
        env = env->next;
    }
    return NULL;
}

static void set_env_value1(t_env *env, const char *key, const char *value)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
        {
            free(env->value);
            env->value = strdup(value);
            return;
        }
        env = env->next;
    }

    t_env *new = malloc(sizeof(t_env));
    new->key = strdup(key);
    new->value = strdup(value);
    new->next = env;
    env = new;
}

int builtin_cd(t_env *env, char **args)
{
    char *target = NULL;
    char cwd[PATH_MAX];

    if (!args[1] || strcmp(args[1], "--") == 0)
    {
        target = get_env_value1(env, "HOME");
        if (!target)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }
    }
    else if (strcmp(args[1], "-") == 0)
    {
        target = get_env_value1(env, "OLDPWD");
        if (!target)
        {
            fprintf(stderr, "cd: OLDPWD not set\n");
            return 1;
        }
        printf("%s\n", target);
    }
    else if (args[2])
    {
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }
    else
    {
        target = args[1];
    }

    if (getcwd(cwd, sizeof(cwd)) != NULL)
        set_env_value1(env, "OLDPWD", cwd);

    if (chdir(target) != 0)
    {
        perror("cd");
        return 1;
    }

    if (getcwd(cwd, sizeof(cwd)) != NULL)
        set_env_value1(env, "PWD", cwd);

    return 0;
}
