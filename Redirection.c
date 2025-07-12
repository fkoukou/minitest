#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void rediriger_entree(const char *fichier) {
    int fd = open(fichier, O_RDONLY);
    if (fd < 0) { perror("open infile"); return; }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void rediriger_sortie(const char *fichier) {
    int fd = open(fichier, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) { perror("open outfile"); return; }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

void rediriger_sortie_append(const char *fichier) {
    int fd = open(fichier, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) { perror("open append"); return; }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

// void rediriger_heredoc(const char *contenu) {
//     int pipefd[2];
//     pipe(pipefd);
//     if (fork() == 0) {
//         close(pipefd[0]);
//         write(pipefd[1], contenu, strlen(contenu));
//         close(pipefd[1]);
//         _exit(0);
//     } else {
//         close(pipefd[1]);
//         dup2(pipefd[0], STDIN_FILENO);
//         close(pipefd[0]);
//     }
// }

// void rediriger_heredoc(const char *delimiter) {
//     int pipefd[2];
//     pipe(pipefd);

//     pid_t pid = fork();
//     if (pid == 0) {
//         // Processus fils : écrire dans le pipe
//         close(pipefd[0]); // fermer la lecture

//         char *line = NULL;
//         size_t len = 0;

//         while (1) {
//             write(STDOUT_FILENO, "> ", 2);
//             if (getline(&line, &len, stdin) == -1) {
//                 break; // EOF ou erreur
//             }

//             // Supprimer le '\n' final
//             line[strcspn(line, "\n")] = '\0';

//             if (strcmp(line, delimiter) == 0) {
//                 break; // Fin du heredoc
//             }

//             // Réécrire la ligne avec '\n' dans le pipe
//             write(pipefd[1], line, strlen(line));
//             write(pipefd[1], "\n", 1);
//         }
//         free(line);
//         close(pipefd[1]);
//         _exit(0);
//     } else {
//         // Processus parent : rediriger stdin vers la lecture du pipe
//         close(pipefd[1]); // fermer l'écriture
//         dup2(pipefd[0], STDIN_FILENO);
//         close(pipefd[0]);
//         wait(NULL);
//     }
// }