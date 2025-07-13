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
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <sys/wait.h>
// #include <string.h>
// #include "minishell.h"
// // Redirection de stdin (< fichier)
// void rediriger_entree(const char *fichier) {
//     int fd = open(fichier, O_RDONLY);
//     if (fd < 0) {
//         perror("open infile");
//         exit(EXIT_FAILURE);
//     }
//     if (dup2(fd, STDIN_FILENO) < 0) {
//         perror("dup2 infile");
//         exit(EXIT_FAILURE);
//     }
//     close(fd);
// }

// // Redirection de stdout (> fichier, écrasement)
// void rediriger_sortie(const char *fichier) {
//     int fd = open(fichier, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     if (fd < 0) {
//         perror("open outfile");
//         exit(EXIT_FAILURE);
//     }
//     if (dup2(fd, STDOUT_FILENO) < 0) {
//         perror("dup2 outfile");
//         exit(EXIT_FAILURE);
//     }
//     close(fd);
// }

// // Redirection de stdout (>> fichier, ajout)
// void rediriger_sortie_append(const char *fichier) {
//     int fd = open(fichier, O_WRONLY | O_CREAT | O_APPEND, 0644);
//     if (fd < 0) {
//         perror("open outfile append");
//         exit(EXIT_FAILURE);
//     }
//     if (dup2(fd, STDOUT_FILENO) < 0) {
//         perror("dup2 outfile append");
//         exit(EXIT_FAILURE);
//     }
//     close(fd);
// }

