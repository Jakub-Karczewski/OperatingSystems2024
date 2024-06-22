#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <stdbool.h>

#define SIZE 100
#define MSG 10
#define READ 2

struct kolej
{
    int p;
    int k;
    char Q[SIZE];
    char queue_i[READ][11];
    int space_full;
    int wake_up;
    bool read_vis[READ];
    int no_readers;
    int no_writers;
    int busy_readers;
};

char * slice(char * s, int a, int b)
{
    char * res = (char*) malloc(sizeof(char) * (b - a + 2));
    for(int i = 0; i < b-a+1; i++)
    {
        res[i] = s[(a + i) % SIZE];
    }
    res[b-a+1] = '\0';
    return res;
}

int main(int argc, char * argv[1])
{
    int tryb = atoi(argv[1]);
    int my_nr;

    int fd1 = shm_open("/struktura", O_CREAT | O_RDWR, 0777);

    int x1 = ftruncate(fd1, sizeof(struct kolej));

    struct kolej * k1 = mmap(NULL, sizeof(struct kolej), PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);

    sem_t * wake = sem_open("wake", O_RDWR, 0777, 0);
    sem_t * remain = sem_open("remain", O_RDWR, 0777, MSG);
    sem_t * busy_read = sem_open("busy_read", O_RDWR, 0777, 1);
    sem_t * suma = sem_open("busy_read", O_RDWR, 0777, 1);
    sem_t * vis = sem_open("vis", O_RDWR, 0777, 1);
    sem_t * synch = sem_open("synch", O_RDWR, 0777, 1);

    k1->no_readers += 1;
    my_nr = k1->no_readers - 1;
    sem_t ** arr;
    
    if(tryb == -1)
    {
        sem_unlink("increase_beg");
        int bufor;

        FILE* in = fopen("potok2", "r");
        sem_t * remain = sem_open("remain", O_RDWR, 0777, 0);

        fread(&bufor, sizeof(int*), 1, in);
        sleep(1.0);
        printf("bufor = %d\n", bufor);

        for(int i = 0; i < bufor + k1->no_readers - 1; i++)
        {
            sem_post(wake);
        }
    }
    else
    {
        sem_wait(wake);
        printf("Drukarka nr %d budzi sie\n", my_nr);
    }

    sem_t * increase_beg = sem_open("increase_beg", O_CREAT | O_RDWR, 0777, 1);
    
    int JD;
    printf("woken up\n");

    while(1)
    {
        sem_wait(suma);
        //printf("suma\n");
        


        sem_getvalue(remain, &JD);
        //printf("JD = %d\n", JD);
        if(JD == MSG)
        {
            for(int i = 0; i < 10; i++)
            {
                printf("%c", k1->queue_i[my_nr][i]);
                fflush(stdout);
                sleep(1);
            }
            printf("\n");

            sem_wait(vis);
            k1->read_vis[my_nr] = true;
            sem_post(vis);

            sem_wait(busy_read);
            k1->busy_readers -= 1;
            sem_post(busy_read);

            sem_getvalue(remain, &JD);

        }
        else
        {
            sem_wait(increase_beg);
            int prev_beg = k1->p;
            char * act = slice(k1->Q, (prev_beg), (prev_beg + 9));
            //printf("%ld\n", strlen(act));
            k1->p += 10;
            sem_post(increase_beg);

            for(int i = 0; i < 10; i++)
            {
                printf("%c", act[i]);
                fflush(stdout);
                sleep(1);
            }
            printf("\n");
            sem_post(remain);
        }
    }
    return 0;
}