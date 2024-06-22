#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <wait.h>
#include <signal.h>

void check_error(int x)
{
    if(x == -1)
    {
        perror("");
    }
    return;
}


#define SIZE 100
#define MSG 10

int main(int argc, char* argv[1])
{
    char snd[14];
    char rcv[14];
    int qi, prior, xi, str_end;

    struct mq_attr attr = {.mq_maxmsg = MSG, .mq_msgsize = SIZE};
    struct mq_attr control;

    sprintf(snd, "/client%s_snd", argv[1]);
    sprintf(rcv, "/client%s_rcv", argv[1]);

    //printf("client_snd = %s\n", snd);
    //printf("client_rcv = %s\n", rcv);
    
    mq_unlink(snd);
    mq_unlink(rcv);

    mqd_t q_snd = mq_open(snd, O_CREAT | O_RDWR, 0777, &attr);
    check_error(q_snd);

    mqd_t q_rcv = mq_open(rcv, O_CREAT | O_RDWR, 0777, &attr);
    check_error(q_rcv);

    char mess[SIZE], snd_buff[SIZE], rcv_buff[SIZE];
    sprintf(mess, "INIT /client%s_rcv /client%s_snd", argv[1], argv[1]);

    mqd_t main_q = mq_open("/main_q", O_CREAT | O_RDWR, 0777, &attr);
    check_error(main_q);

    xi = mq_send(main_q, mess, SIZE, 0);
    check_error(xi);

    mq_getattr(q_rcv, &control);

    printf("Wiadomosci: %ld\n", control.mq_curmsgs);

    xi = mq_receive(q_rcv, rcv_buff, SIZE, &prior);
    check_error(xi);

    printf("Numer w kolejce: %s\n", rcv_buff);

    pid_t child;
    child = fork();
    if(child == 0)
    {
        char child_buffer[SIZE];
        while(1)
        {
            xi = mq_receive(q_rcv, child_buffer, SIZE, &prior);
            check_error(xi);

            if(strcmp("terminate", child_buffer) == 0)
            {
                exit(0);
            }

            printf("%s\n", child_buffer);
        }
    }
    else
    {
        char parent_buffer[SIZE];
        while(1)
        {
            scanf("%s", parent_buffer);

            if(strcmp("terminate", parent_buffer) == 0)
            {
                exit(0);
            }

            xi = mq_send(q_snd, parent_buffer, SIZE, 0);
            check_error(xi);
        }
    }
    return 0;
}