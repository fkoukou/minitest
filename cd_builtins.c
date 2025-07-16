/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fakoukou <fakoukou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 06:16:32 by fakoukou          #+#    #+#             */
/*   Updated: 2025/07/16 06:39:51 by fakoukou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_value1(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static void	set_env_value1(t_env *env, const char *key, const char *value)
{
	t_env	*new;

	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			free(env->value);
			env->value = strdup(value);
			return ;
		}
		env = env->next;
	}
	new = malloc(sizeof(t_env));
	new->key = strdup(key);
	new->value = strdup(value);
	new->next = env;
	env = new;
}

static char	*get_cd_target(t_env *env, char **args)
{
	char	*home;
	char	*oldpwd;

	if (!args[1] || strcmp(args[1], "--") == 0)
	{
		home = get_env_value1(env, "HOME");
		if (!home)
			fprintf(stderr, "cd: HOME not set\n");
		return (home);
	}
	if (strcmp(args[1], "-") == 0)
	{
		oldpwd = get_env_value1(env, "OLDPWD");
		if (!oldpwd)
			fprintf(stderr, "cd: OLDPWD not set\n");
		else
			printf("%s\n", oldpwd);
		return (oldpwd);
	}
	if (args[2])
	{
		fprintf(stderr, "cd: too many arguments\n");
		return (NULL);
	}
	return (args[1]);
}

static void	update_pwd_vars(t_env *env, const char *oldpwd)
{
	char	cwd[PATH_MAX];

	set_env_value1(env, "OLDPWD", oldpwd);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		set_env_value1(env, "PWD", cwd);
}

int	builtin_cd(t_env *env, char **args)
{
	char	*target;
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, sizeof(cwd)))
		cwd[0] = '\0';
	target = get_cd_target(env, args);
	if (!target)
		return (1);
	if (chdir(target) != 0)
	{
		perror("cd");
		return (1);
	}
	update_pwd_vars(env, cwd);
	return (0);
}
