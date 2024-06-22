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

#define SIZE 100
#define MSG 10

char *read_till(int a, char stop, char *str, int *b)
{
    int k = a;
    while (str[k] != stop)
    {
        k++;
    }
    int dl = k - a;
    char *dupa = (char *)malloc((dl + 1) * sizeof(char));

    for (int j = a; j < k; j++)
    {
        dupa[j - a] = str[j];
    }

    dupa[k - a] = '\0';
    *b = k;
    return dupa;
}

void check_error(int x)
{
    if(x == -1)
    {
        perror("");
    }
    return;
}

int main()
{
    int n = 2;
    struct mq_attr attr = {.mq_maxmsg = MSG, .mq_msgsize = SIZE};
    struct mq_attr control;

    mq_unlink("/main_q");
    mqd_t main_q = mq_open("/main_q", O_CREAT | O_RDWR, 0777, &attr);
    check_error(main_q);


    char send_i[SIZE];
    char rcv_i[SIZE];
    int** desc = (int **) malloc(sizeof(int*) * n);
    for(int i = 0; i < n; i++)
    {
        desc[i] = (int*) malloc(sizeof(int) * 2);
    }

    int xi, prior, str_end;
    for(int i = 0; i < n; i++)
    {
        xi = mq_receive(main_q, rcv_i, SIZE, &prior);
        check_error(xi);

        char* mess1 = read_till(5, ' ', rcv_i, &str_end);
        char* mess2 = read_till(str_end + 1, '\0', rcv_i, &xi);

        //printf("rcv = %s\nsnd = %s\n", mess1, mess2);
        
        mqd_t qi_rcv = mq_open(mess1, O_CREAT | O_RDWR, 0777, &attr);
        check_error(qi_rcv);
        mqd_t qi_send = mq_open(mess2, O_CREAT | O_RDWR, 0777, &attr);
        check_error(qi_send);

        desc[i][0] = qi_rcv;
        desc[i][1] = qi_send;

        sprintf(send_i, "%d", i+1);
        xi = mq_send(qi_rcv, send_i, SIZE, 0);
        check_error(xi);

    }

    char bufor[SIZE];
    char others[SIZE];
    while(1)
    {
        for(int i = 0; i < n ; i++)
        {
            mq_getattr(desc[i][1], &control);
            long steps = control.mq_curmsgs;
            if(steps > 0)
            {
                for(int j = 0; j < steps; j++)
                {
                    xi = mq_receive(desc[i][1], bufor, SIZE, &prior);
                    check_error(xi);
                    printf("client %d: %s\n", i+1, bufor);
                    if(strcmp("break", bufor) == 0)
                    {
                        exit(0);
                    }
                    char* raw = read_till(0, '\0', bufor, &prior);
                    sprintf(others, "client%d: %s", i+1, raw);
                    for( int k = 0; k < n; k++)
                    {
                        if(k != i)
                        {
                            xi = mq_send(desc[k][0], others, SIZE, 0);
                            check_error(xi);
                        }
                    }
                }
            }
        }
    }
    
    printf("Rejestracja zakonczona\n");
    return 0;  
}
