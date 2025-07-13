#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include "minishell.h"


// Fonctions de redirection déjà définies dans ton code :
// rediriger_entree, rediriger_sortie, rediriger_sortie_append

// Analyse args pour redirections, retire les tokens redirections et fichiers
// et retourne les fichiers d'entrée et sortie via pointeurs.
void parse_redirections(char **args, char **infile, char **outfile, int *append)
{
    *infile = NULL;
    *outfile = NULL;
    *append = 0;

    int i = 0;
    while (args[i])
    {
        if (strcmp(args[i], "<") == 0)
        {
            *infile = args[i + 1];
            // Décaler args pour retirer "<" et fichier
            while (args[i + 2])
            {
                args[i] = args[i + 2];
                i++;
            }
            args[i] = NULL;
            args[i + 1] = NULL;
            i = 0; // restart scanning
            continue;
        }
        else if (strcmp(args[i], ">") == 0)
        {
            *outfile = args[i + 1];
            *append = 0;
            while (args[i + 2])
            {
                args[i] = args[i + 2];
                i++;
            }
            args[i] = NULL;
            args[i + 1] = NULL;
            i = 0;
            continue;
        }
        else if (strcmp(args[i], ">>") == 0)
        {
            *outfile = args[i + 1];
            *append = 1;
            while (args[i + 2])
            {
                args[i] = args[i + 2];
                i++;
            }
            args[i] = NULL;
            args[i + 1] = NULL;
            i = 0;
            continue;
        }
        i++;
    }
}

// Exécute la commande avec redirections
void execute_command(char **args, t_env *env_list)
{
    pid_t pid;
    int status;
    char *infile = NULL;
    char *outfile = NULL;
    int append = 0;

    // Analyse redirections et modifie args en place
    parse_redirections(args, &infile, &outfile, &append);

    pid = fork();
    if (pid == 0)
    {
        // Processus fils

        // Appliquer redirection entrée si besoin
        if (infile)
            rediriger_entree(infile);

        // Appliquer redirection sortie si besoin
        if (outfile)
        {
            if (append)
                rediriger_sortie_append(outfile);
            else
                rediriger_sortie(outfile);
        }

        // Exécuter la commande (chercher dans PATH etc.)
        // Ici tu peux appeler ta fonction existante
        execute_external(args, env_list);

        // Si exec échoue
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        // Processus parent attend le fils
        waitpid(pid, &status, 0);
    }
    else
    {
        perror("fork");
    }
}
