#include <stdlib.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void handler(int sig_no)
{
    printf("\nWykonuje handler:\nOtrzymany sygnal = %d\n\n", sig_no);
}

int main(int argc, char* argv[1])
{
    char* mode = argv[1];

    //printf("argv[1] = %s\n", mode);

    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    if(strcmp(mode, "handler") == 0)
    {
        act.sa_handler = handler;
        sigaction(SIGUSR1, &act, NULL);
        raise(SIGUSR1);
    }
    else if(strcmp(mode, "ignore") == 0)
    {
        act.sa_handler = SIG_IGN;
        sigaction(SIGUSR1, &act, NULL);
        raise(SIGUSR1);
    }
    else if(strcmp(mode, "mask") == 0)
    {
        sigset_t zbior;
        sigemptyset(&zbior);
        sigaddset(&zbior, SIGUSR1);
        sigprocmask(SIG_BLOCK, &zbior, NULL);

        act.sa_handler = handler;
        sigaction(SIGUSR1, &act, NULL);
        raise(SIGUSR1);

        sigpending(&zbior);
        printf("Czy maska blokowanych zawiera SIGUSR1: %d\n", sigismember(&zbior, SIGUSR1));
    }
    return 0;
}