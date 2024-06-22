#include <stdlib.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int convert(char* s)
{
    int i = 0;
    while(s[i] != '\0')
        i++;
    int pot = 1;
    int num = 0;
    for(int k = i-1; k >= 0; k--)
    {
        num += pot * (s[k] - '0');
        pot *= 10;
    }
    return num;
}

void handler_sender(int sig_no)
{
    printf("Sender zakonczyl dzialanie\n");
}


int main(int argc, char* argv[2])
{

    int tryb = argv[2][0] - '0';
    pid_t catcher_ID = convert(argv[1]);

    printf("pid catchera = %d\n", catcher_ID);
    printf("Tryb sendera = %d\n", tryb);

    union sigval send;
    send.sival_int = tryb;
    sigqueue(catcher_ID, SIGUSR1, send);

    struct sigaction confirm;
    confirm.sa_flags = 0;
    confirm.sa_handler = handler_sender;
    sigemptyset(&confirm.sa_mask);
    sigaddset(&confirm.sa_mask, SIGUSR1);
    sigaction(SIGUSR1, &confirm, NULL);

    sigset_t waiting;
    sigemptyset(&waiting);
    sigfillset(&waiting);
    sigdelset(&waiting, SIGUSR1);

    sigsuspend(&waiting);

    return 0;
}