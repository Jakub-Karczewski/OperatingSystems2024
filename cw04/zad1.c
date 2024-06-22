#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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

int main(int argc, char * argv[1])
{
    pid_t ID;
    int JD;
    int n = convert(argv[1]);
    for(int i = 1; i <= n; i++)
    {
        ID = fork();
        wait(&JD);
        if(ID == 0)
        {
            printf("i = %d\nID: %d\nParentID = %d\n", i, getpid(), getppid());
            break;
        }
        sleep(0.5);
    }

    if(ID != 0)
    {
        printf("\nargv[1] = %d\n\n\n", n);
    }
    return 0;
}