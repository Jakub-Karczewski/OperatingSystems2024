#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <time.h>

int ren_count = 0, R = 9, M = 1;
int santa_quit = 0;
int u_sec = 0;

pthread_mutex_t r_num;
pthread_mutex_t santa_end;
pthread_cond_t present;
pthread_t * rr;

void handler1(int sig_no)
{
    return;
}
void renifer(int ren_i)
{
    while(1)
    {
        int t1 = 2 + rand() % 3;
        sleep(t1);
        pthread_mutex_lock(&r_num);
        ren_count += 1;
        //printf("czas: %d\n", t1);
        printf("%d reniferow czeka na mikolaja\n", ren_count);
        if(ren_count == 9)
        {
            printf("Wybudzam mikolaja\n");
            pthread_cond_broadcast(&present);
        }
        pthread_mutex_unlock(&r_num);

        struct sigaction act;
        act.sa_handler = handler1;
        sigaction(SIGUSR1, &act, NULL);
        pause();
        sleep(u_sec);
    }

}

void santa(int santa_i)
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    for(int i = 0; i < 4; i++)
    {
        pthread_mutex_lock(&r_num);
        while(ren_count < 9)
        {
            //printf("Mikolaj czeka\n");
            pthread_cond_wait(&present, &r_num);
        }
        pthread_mutex_unlock(&r_num);

        printf("Mikolaj budzi sie\n");
        u_sec = 2 + rand() % 3;
        for(int i = 0; i < R; i++)
        {
            pthread_kill(rr[i], SIGUSR1);
        }
        printf("Mikolaj dostarcza prezenty\n");
        ren_count = 0;
        sleep(u_sec);
        printf("Mikolaj zasypia\n");

    }

    printf("MIKOLAJ %d KONCZY\n", santa_i);
    for(int i = 0; i < R; i++)
    {
        pthread_cancel(rr[i]);
    }
}

int main()
{
    srand(time(NULL));
    int M = 1;
    rr = (pthread_t *) malloc(sizeof(pthread_t) * R);
    pthread_mutex_init(&r_num, NULL);
    pthread_mutex_init(&santa_end, NULL);
    pthread_cond_init(&present, NULL);

    pthread_t * mm = (pthread_t *) malloc(sizeof(pthread_t) * M);
    for(int i = 0; i < R; i++)
    {
        pthread_create(&rr[i], NULL, (void*) renifer, (void*) i);
    }
    for(int i = 0; i < M; i++)
    {
        pthread_create(&mm[i], NULL, (void*) santa, (void*) i);
    }

    int x;
    for(int i = 0; i < R; i++)
    {
        pthread_join(rr[i], (void**) &x);
    }
    for(int i = 0; i < M; i++)
    {
        pthread_join(mm[i], (void**) &x);
    }

    free(rr);
    free(mm);
    pthread_mutex_destroy(&r_num);
    pthread_mutex_destroy(&santa_end);
    pthread_cond_destroy(&present);
    return 0;
}