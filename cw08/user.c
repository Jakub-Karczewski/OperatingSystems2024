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

void check_err(int x)
{
    if(x == -1)
    {
        perror("");
    }
}

void check_errno()
{
    if(errno > 0)
    {
        perror("");
    }
}

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

struct kolej
{
    int p;
    int k;
    char Q[SIZE+1];
    char queue_i[READ][11];
    int space_full;
    int wake_up;
    bool read_vis[READ];
    int no_readers;
    int no_writers;
    int busy_readers;
};

int main(int argc, char* argv[1])
{
    
    int x1, temp, my_nr;
    int tryb = atoi(argv[1]);

    if(tryb == 0)
    {
        x1 = sem_unlink("wake");
        x1 = sem_unlink("remain");
        x1 = sem_unlink("ready");
        x1 = sem_unlink("busy_read");
        x1 = sem_unlink("vis");
        x1 = sem_unlink("synch");
        for(int i = 0; i < READ; i++)
        {
            char name_i[3];
            sprintf(name_i, "c%d", i);
            x1 = sem_unlink(name_i);
        }

        x1 = shm_unlink("/struktura");

    }

    int fd1 = shm_open("/struktura", O_CREAT | O_RDWR, 0777);
    x1 = ftruncate(fd1, sizeof(struct kolej));
    check_err(x1);
    struct kolej *k1 = mmap(NULL, sizeof(struct kolej), PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);

    if(k1 == MAP_FAILED)
    {
        perror("");
    }

    k1->Q[SIZE] = '\0';

    sem_t * remain = sem_open("remain", O_CREAT | O_RDWR, 0777, MSG);
    sem_t * ready = sem_open("ready", O_CREAT | O_RDWR, 0777, 1);
    sem_t * wake = sem_open("wake", O_CREAT | O_RDWR, 0777, 0);
    sem_t * busy_read = sem_open("busy_read", O_CREAT | O_RDWR, 0777, 1);
    sem_t * suma = sem_open("busy_read", O_RDWR | O_CREAT, 0777, 0);
    sem_t * vis = sem_open("vis", O_RDWR | O_CREAT, 0777, 1);
    sem_t * synch = sem_open("synch", O_RDWR | O_CREAT, 0777, 1);

    k1->no_writers += 1;
    my_nr = k1->no_writers - 1;

    if(tryb == -1)
    {
        memset(k1->read_vis, 1, READ);
        for(int i = 0; i < READ; i++)
        {
            printf("%d ", k1->read_vis[i]);
        }
        printf("\n");
        int val = k1->no_writers;
        int send = val;
        FILE * out = fopen("potok2", "w");
        fwrite(&send, sizeof(int*), 1, out);
        fclose(out);

    }

    sem_wait(wake);
    k1->wake_up += 1;

    char*  napis = "abcdefghijklmnoprstuwyz\0";
    char lit = napis[my_nr];
    int JD;

    //printf("len = %ld\n", strlen(k1->Q));

    while(1)
    {
        sem_wait(ready);
        if(k1->busy_readers == k1->no_readers)
        {
            sem_wait(remain);
            //printf("remain\n");

            //printf("Wpisuje do kolejki: ");
            //printf("before = %ld\n", strlen(k1->Q));

            int prev_end = k1->k;
            k1->k += 10;

            for(int i = 0; i < 10; i++)
            {
                k1->Q[(prev_end + i) % SIZE] = lit;
                printf("%ld ", strlen(k1->Q));
            }
            sem_post(suma);
            printf("%ld\n%s\n", strlen(k1->Q), k1->Q);
        }

        else
        {
            int found = -1;
            for(int i = 0; i < READ; i++)
            {
                if(k1->read_vis[i])
                {
                    found = i;

                    sem_trywait(vis);
                    k1->read_vis[i] = false;
                    sem_post(vis);

                    break;
                }
            }

            //printf("found = %d\n", found);

            if(found != -1)
            {
                for(int j = 0; j < 10; j++)
                {
                    k1->queue_i[found][j] = lit;
                }
                //printf("przed busy\n");
                sem_wait(busy_read);
                //printf("busy_read\n");
                k1->busy_readers += 1;
                sem_post(busy_read);
                sem_post(suma);

            }
            //printf("Po ifie\n");
        }
        sem_post(ready);
        sleep(1.0);
    }
    return 0;
}