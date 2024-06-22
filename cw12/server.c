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
#define MAX_CLIENTS 10
#define MAX_SPACE 50
#define VAL_RANGE 200
#define port1 3000

int val_to_ID[VAL_RANGE];
int ID_to_val[MAX_SPACE];
struct sockaddr_in * adresy;

int act_clients = 0;
int act_active = 0;
int sfd;

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

    //printf("\nKoncze serwer\nPOZDRAWIAM LEGNICE DOBRYCH CHLOPAKOW I NIECH SIE TO TRZYMA DOBRY PRZEKAZ LECI\n");
    shutdown(sfd, SHUT_RDWR);
    close(sfd);
    exit(0);
}

int main()
{

    if((sfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){
        perror("socket creation fail");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < MAX_SPACE; i++){
        ID_to_val[i] = -1;
    }

    for(int i = 0; i < VAL_RANGE; i++){
        val_to_ID[i] = -1;
    }

    adresy = (struct sockaddr_in * ) malloc( sizeof(struct sockaddr_in) * MAX_SPACE);
    struct sockaddr_in sv_addr, c_addr;
    socklen_t c_len;

    memset(&sv_addr, 0, sizeof(sv_addr));
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_port = htons(port1);

    if((bind(sfd, (struct sockaddr *) &sv_addr, sizeof(struct sockaddr_in)))  < 0 ){
        perror("bind failed");
    }


    /*
    char err_buffer[BUF_SIZE];
    while(recvfrom(sfd, (char *) err_buffer, BUF_SIZE, MSG_DONTWAIT, (struct sockaddr *) &c_addr, &c_len) != -1) {
        printf("%s\n", err_buffer);
    }
    perror("");
    */
    
    

    char buffer[BUF_SIZE];

    int temp, temp1, temp2, temp3;

    struct sigaction act1;
    act1.sa_handler = handler1;
    sigaction(SIGINT, &act1, NULL);

    while(1) {

        struct sockaddr_in temp_addr;
        socklen_t dlug;
        ssize_t n = recvfrom(sfd, (char *) buffer, BUF_SIZE, 0, (struct sockaddr *) &temp_addr, &dlug);

        //printf("BUFFER: %s\n", buffer);

        int c_val = atoi(read_till(buffer, 0, ' ', &temp));

        if(val_to_ID[c_val] == -1){
            ID_to_val[act_clients] = c_val;
            val_to_ID[c_val] = act_clients;
            adresy[act_clients] = temp_addr;
            act_clients++;
        }

        char * cmd = read_till(buffer, temp+1, ' ', &temp1);
        char * args1 = read_till(buffer, temp1+1, ' ', &temp2);
        char * args2 = read_till(buffer, temp2+1, '\0', &temp3);

        if(strcmp(cmd, "LIST") == 0){

            char napis[BUF_SIZE];
            napis[0] = '\0';

            for(int i = 0; i < act_clients; i++){
                if(ID_to_val[i] >= 0) {
                    char num[3];
                    int k = sprintf(num, "%d", i);
                    num[k] = '\0';
                    strcat(napis, num);
                }
            }

            ssize_t sent1 = sendto(sfd, (char *) napis, strlen(napis) + 1, 0, (const struct sockaddr * ) &temp_addr, sizeof(temp_addr));
            if(sent1 == -1){
                perror("sent1 failed");
            }
        }

        else if(strcmp(cmd, "STOP") == 0){
            printf("stop\n");
            val_to_ID[ID_to_val[atoi(args1)]] = -2;
            ID_to_val[atoi(args1)] = -2;
        }

        else if(strcmp(cmd, "2ONE") == 0){
            struct sockaddr_in actual1 = adresy[atoi(args1)];
            ssize_t sent2 = sendto(sfd, (char * ) args2, strlen(args2) + 1, 0, (const struct sockaddr * ) &actual1, sizeof(actual1));
            if(sent2 == -1){
                perror("sent2 failed");
            }
        }

        else if(strcmp(cmd, "2ALL") == 0){
            for(int i = 0; i < act_clients; i++){
                if(ID_to_val[i] >= 0)
                {
                    struct sockaddr_in actual2 = adresy[i];
                    ssize_t sent3 = sendto(sfd, (char *) args1, strlen(args1) + 1, 0,  (const struct sockaddr * ) &actual2, sizeof(actual2));
                    if(sent3 == -1){
                        perror("sent3 failed");
                    }
                }
            }
        }
    }

    shutdown(sfd, SHUT_RDWR);
    close(sfd);
    return 0;
}