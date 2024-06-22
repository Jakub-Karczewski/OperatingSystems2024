#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <pthread.h>
#include <signal.h>

#define SV_SOCK_PATH "/tmp/sock"
#define BUF_SIZE 100

int sfd;
int my_nr;
pthread_t reading;

struct message
{
    int * list;
    char * m;
    bool what;
};

void handler(int sig_no)
{
    printf("Koncze klienta %d\n", my_nr);
    close(sfd);
    //pthread_cancel(reading);
    exit(0);
}

void f()
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    ssize_t numRead;
    struct message buffer;

    while(1)
    {
        numRead  = read(sfd, &buffer, sizeof(buffer));
        if(buffer.what == true){
            int n = buffer.list[0];
            printf("Wypisuje liste aktywnych\n");
            for(int i = 1; i < n+1; i++){
                printf("%d ", buffer.list[i]);
            }
        }
        else
        {
            printf("Wypisuje string\n");
            printf("%s\n", buffer.m);
        }
    }
}


int main(int argc, char * argv[2])
{
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    int serv_port = atoi(argv[1]);
    my_nr = atoi(argv[2]);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(serv_port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(connect(sfd, (struct sockaddr * ) &addr, sizeof(struct sockaddr_in)) == -1)
    {
        perror("connect");
    }

    printf("1\n");

    char buf[BUF_SIZE];
    ssize_t numRead;

    pthread_create(&reading, NULL, (void*) f, NULL);
    
    while((numRead = read(STDIN_FILENO, buf, BUF_SIZE) ) > 0)
    {
        if(write(sfd, buf, numRead) != numRead)
        {
            perror("partial/failed write");
        }
    }

    close(sfd);
    return 0;
}