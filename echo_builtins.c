/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fakoukou <fakoukou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 06:44:47 by fakoukou          #+#    #+#             */
/*   Updated: 2025/07/16 06:54:50 by fakoukou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int builtin_echo(char **args, t_env *env_list)
{
    bool n_flag = false;
    int i = 1;

    (void)env_list; // inutilisé ici

    // Vérifier si option -n (peut être plusieurs n, ex: -nnn)
    while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
    {
        int j = 2;
        while (args[i][j] == 'n')
            j++;
        if (args[i][j] != '\0')
            break;
        n_flag = true;
        i++;
    }

    while (args[i])
    {
        if (strcmp(args[i], "$?") == 0)
            printf("%d", g_exit_status);
        else
            printf("%s", args[i]);

        if (args[i + 1])
            printf(" ");
        i++;
    }

    if (!n_flag)
        printf("\n");

    return 0;
}

