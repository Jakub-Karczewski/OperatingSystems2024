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
#define  max_clients 10
#define GIGA 1000
#define SV_SOCK_PATH "/tmp/sock"

pthread_mutex_t fout;
pthread_mutex_t vis;
pthread_cond_t full;
pthread_t * watki;

struct message
{
    int * list;
    char * m;
    bool what;
};

int act_clients;
int act_index;
int sfd;


char * slice(char * A, int p, int k){
    char * new = (char*) malloc(sizeof(char) * (k - p + 2));
    for(int i = p; i <= k; i++){
        new[i-p] = A[i];
    }
    new[k-p + 1] = '\0';
    return new;
}

char* read_until(char * A, int beg, char stop, int *ended)
{
    int n = 0, dl = strlen(A);
    bool flag = false;
    for(int i = beg; i < dl; i++){
        if(A[i] == stop){
            flag = true;
            break;
        }
        n++;
    }
    *ended = (flag) ? beg + n : dl;
    char * res = (char*) malloc(sizeof(char) * n);
    for(int i = 0; i < n; i++){
        res[i] = A[i + beg];
    }
    return res;
}

void copy_tables(int * src, int * dest, int n){
    for(int i = 0; i < n; i++){
        dest[i] = src[i];
    }
}

void handler(int sig_no)
{
    shutdown(sfd, SHUT_RDWR);
    close(sfd);
}


int active[BUF_SIZE];
int all_cfd[BUF_SIZE];

void server_rcv(int args[2])
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    int my_nr = args[0];
    int cfd = args[1];
    printf("Watek nr %d: %d\n", my_nr, cfd);

    char buffer[BUF_SIZE];
    int temp;

    ssize_t numRead;
    while((numRead = read(cfd, buffer, BUF_SIZE)) > 0)
    {
        
        if(strcmp(buffer, "LIST") == 0)
        {
            int * lista_toCopy = (int*) malloc( sizeof(int) * (BUF_SIZE + 1));
            lista_toCopy[0] = act_index;
            int count = 1;

            for(int i = 0; i < BUF_SIZE; i++){
                if(active[i]){
                    printf("%d ", i);
                    lista_toCopy[count] = i;
                    count++;
                }
            }
            printf("\n");

            struct message mess1;
            copy_tables(lista_toCopy, mess1.list, count);
            mess1.what = true;

            write(cfd, &mess1, sizeof(mess1));

            pthread_mutex_unlock(&fout);
        }

        else if(strcmp(buffer, "STOP") == 0){

            pthread_mutex_lock(&vis);

            act_clients--;
            if(act_clients == max_clients - 1){
                pthread_cond_broadcast(&full);
            }
            active[my_nr] = 0;

            pthread_mutex_unlock(&vis);
            pthread_cancel(pthread_self());
        }

        else if(strcmp(buffer, "ALIVE") == 0){
            // jeszcze niegotowe
        }
        else if(strcmp( slice(buffer, 0, 3 ), "2ALL")){

            struct message mess2;
            mess2.what = false;

            char * to_all = read_until(buffer, 5, ' ', &temp);
            sprintf(mess2.m, "%s", to_all);

            for(int i = 0; i < BUF_SIZE; i++){
                if(active[i]){
                    write(all_cfd[i], &mess2, sizeof(mess2));
                }
            }

        }
        else if(strcmp( slice(buffer, 0, 3 ), "2ONE")){

            struct message mess3;
            mess3.what = false;

            char * ID_str = read_until(buffer, 5, ' ', &temp);
            int ID = atoi(ID_str);

            char * to_one = read_until(buffer, temp + 1, ' ', &temp);
            sprintf(mess3.m, "%s", to_one);

            write(all_cfd[ID], to_one, strlen(to_one) + 1);
        }
        else{
            perror("Invalid message");
        }
    }
    printf("Koncze watek %d\n", my_nr);
}

int main(int argc, char* argv[1])
{
    printf("%s\n", argv[1]);

    pthread_mutex_init(&fout, NULL);
    pthread_mutex_init(&vis, NULL);
    pthread_cond_init(&full, NULL);
    
    int serv_port = atoi(argv[1]);
    int ret;

    remove(SV_SOCK_PATH);
    sfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(serv_port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sfd,(struct sockaddr *) &addr, sizeof(struct sockaddr_in))==-1){
        perror("bind");
    }

    if(listen(sfd,1)==-1){
        perror("listen");
    }

    ssize_t numRead;
    char buf[BUF_SIZE];

    watki = (pthread_t*) malloc(sizeof(pthread_t) * BUF_SIZE);
    int capacity = max_clients;
    
    while(1)
    {
        int cfd = accept(sfd, NULL, NULL);
        printf("accepted socket fd=%d\n",cfd);  
        pthread_mutex_lock(&vis);

        while(act_clients >= max_clients){
            pthread_cond_wait(&full, &vis);
        }

        act_clients++;
        active[act_index] = 1;
        all_cfd[act_index] = cfd;
        int args[2] = {act_index, all_cfd[act_index]};

        pthread_create(&watki[act_index], NULL, (void*) server_rcv, (void*) args);
        pthread_join(watki[act_index], (void**) ret);

        act_index++;

        pthread_mutex_unlock(&vis);

    }

    shutdown(sfd, SHUT_RDWR);
    close(sfd);
    return 0;
}