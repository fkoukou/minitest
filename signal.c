#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>

void sigint_handler(int signo)
{
    (void)signo;
    write(1, "\n", 1);           // saut de ligne
    rl_on_new_line();           // readline à une nouvelle ligne
    rl_replace_line("", 0);     // efface la ligne courante
    rl_redisplay();             // rafraîchit le prompt + ligne vide
}


void setup_signal_handlers(void)
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;  // important pour readline
    sigaction(SIGINT, &sa, NULL);

    // Ignorer SIGQUIT (Ctrl-\)
    signal(SIGQUIT, SIG_IGN);
}
