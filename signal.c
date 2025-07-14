#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>

void sigint_handler(int signo)
{
    (void)signo;
    write(1, "\n", 1);          
    rl_on_new_line();           
    rl_replace_line("", 0);     
    rl_redisplay();             
}


void setup_signal_handlers(void)
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;  
    sigaction(SIGINT, &sa, NULL);

    signal(SIGQUIT, SIG_IGN);
}
