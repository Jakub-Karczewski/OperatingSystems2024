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

#define BUF_SIZE 100
#define PORT1 3000
int sfd;

pthread_t receiver;
struct sockaddr_in sv_addr, c_addr;

void receiving(){

    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    char buff_th[BUF_SIZE];
    struct sockaddr_in svaddr_th;
    socklen_t got_size;
    
    while(1){
        ssize_t readed = recvfrom(sfd, (char * ) buff_th, BUF_SIZE, 0, (struct sockadrr *) &svaddr_th, &got_size);
        if(readed == -1){
            continue;
        }
        printf("buffer: %s\n", buff_th);
        fflush(stdout);
    }
}

char * read_till(char * A, int beg, char stop, int * last)
{
    int n = 0;
    for(int i = beg; i < strlen(A); i++){
        if(A[i] == stop){
            break;
        }
        n++;
    }
    *last = beg + n;
    char * res = (char*) malloc(sizeof(char) * n);
    for(int i = 0; i < n; i++){
        res[i] = A[i+beg];
    }
    return res;
}


void handler1(int sig_no){

    char buff_hand[BUF_SIZE];

    sprintf(buff_hand, "%s", "STOP");
    ssize_t x = sendto(sfd, (const char *) buff_hand, strlen(buff_hand) + 1, 0, (const struct sockaddr*) &sv_addr, sizeof(sv_addr));

    shutdown(sfd, SHUT_RDWR);
    close(sfd);

    pthread_cancel(receiver);
    //printf("\nKoncze klienta\nDOBRZE CHLOPAKI ROBIA WSZYSTKO JEST W PORZADKU DOBRZE ROBIA \n");
    exit(0);
}

int main()
{

    if((sfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){
        perror("socket creation fail");
        exit(EXIT_FAILURE);
    }

    memset(&sv_addr, 0, sizeof(sv_addr));
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_port = htons(PORT1);

    if(connect(sfd, (const struct sockaddr *) &sv_addr, (socklen_t) sizeof(sv_addr)) == -1){
        perror("connect failed");
    }

    char buffer[BUF_SIZE];
    socklen_t len;
    ssize_t num;
    int temp, temp1;

    pthread_create(&receiver, NULL, (void*) receiving, (void * ) 0);

    struct sigaction ctrl_c;
    ctrl_c.sa_handler = handler1;
    sigaction(SIGINT, &ctrl_c, NULL);
    while(1)
    {
        scanf("%[^\n]%*c", buffer);

        int c_val = atoi(read_till(buffer, 0, ' ', &temp));
        char * c_word = read_till(buffer, temp+1, ' ', &temp1);

        if(strcmp(c_word, "STOP") == 0){
            kill(getpid(), SIGINT);
        }

        //printf("GOT: %s\n", buffer);

        num = sendto(sfd, (const char *) buffer, strlen(buffer) + 1, 0, (const struct sockaddr*) &sv_addr, sizeof(sv_addr));
    }

    return 0;
}