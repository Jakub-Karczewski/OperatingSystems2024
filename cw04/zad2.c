#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int gl = 5;

char* slice(char * napis, int a, int b)
{   
    char *new_string = (char*) malloc(sizeof(char) * (b-a + 2));
    for(int i = 0; i < (b-a+1); i++)
    {
        new_string[i] = napis[i+a];
    }
    new_string[b-a+1] = '\0';
    return new_string;
}

int main(int argc, char* argv[1])
{
    pid_t ID;
    int status;
    int dl = 0;
    while(argv[0][dl] != '\0')
        dl += 1;
    printf("\nNazwa programu: %s\n\n", slice(argv[0], 2, dl-1));
    int loc = 10;
    ID = fork();
    if(ID == 0)
    {
        printf("child process:\n");
        gl++;
        loc++;
        printf("child pid = %d\nparent pid = %d\n", getpid(), getppid());
        printf("child's local = %d\nchild's global = %d\n\n", loc, gl);
        execl("/bin/ls", "ls", argv[1], NULL);

    }
    else
    {
        wait(&status);
        printf("\nparent process:\n");
        printf("parent pid = %d\nchild pid = %d\n", getpid(), ID);
        printf("Parent's local = %d\nparent's global = %d\n", loc, gl);
        printf("child exit code: %d\n\n\n", status);
        sleep(1);
        exit(status);
    }

    return 0;  
}
