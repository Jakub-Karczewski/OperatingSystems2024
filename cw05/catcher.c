#include <stdlib.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


volatile int changed = 0;
volatile int mode = 0;


char* int_to_string(int num)
{
    int i, rem, len = 0, n;
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    char *str = (char*) malloc(sizeof(char) * (len+1));
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
    return str;
}


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


void tryb(int no)
{
    int limit = 100;
    if(no == 1){
        printf("\n");
        for(int i = 1; i <= limit; i++)
        {
            printf("%d ", i);
        }
        printf("\n");
    }
    else if(no == 2)
    {
        printf("\nLiczba zmian: %d\n", changed);
    }
    else if(no == 3)
    {
        printf("\nKoncze program catcher\n\n");
        exit(0);
    }
}

void handler_catcher(int sig_no, siginfo_t* info, void*)
{
    int new_mode = info->si_int;
    int sender_PID = info->si_pid;
    if(new_mode != mode && mode != 0)
    {
        changed++;
    }
    mode = new_mode;
    kill(sender_PID, SIGUSR1);
    tryb(new_mode);

}


int main()
{

    pid_t ID = getpid();
    printf("\nCATCHER:\n");
    printf("PID = %d\n", ID);

    struct sigaction catch;
    catch.sa_flags = SA_SIGINFO;
    catch.sa_sigaction  = handler_catcher;

    sigset_t waiting;
    sigemptyset(&waiting);
    sigfillset(&waiting);
    sigdelset(&waiting, SIGUSR1);
    sigemptyset(&catch.sa_mask);


    while(1)
    {
        sigaction(SIGUSR1, &catch, NULL);
        sigsuspend(&waiting);
    }
    return 0;
}





