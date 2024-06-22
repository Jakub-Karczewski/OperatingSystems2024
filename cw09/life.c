#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>


char * foreground;
char * background;
char* tmp;

void handler1(int sig_no)
{
    return;
}


void quit_win(int sig_no)
{
    endwin();
    destroy_grid(foreground);
	destroy_grid(background);
    printf("Zamykam program\n");
    sleep(1);
    exit(0);
}

void f(struct arg* dane)
{
    while(1)
    {
        struct sigaction act;
        act.sa_handler = handler1;
        sigaction(SIGUSR1, &act, NULL);
        pause();
        update_grid(foreground, background, dane);
        //printf("Budze: %d\n", dane->my_nr);
    }
}

int width = 30;
int height = 30;

int main(int argc, char * argv[4])
{
    int n = atoi(argv[1]);
    height = atoi(argv[2]);
    width = atoi(argv[3]);
    int wait_time = atoi(argv[4]);

    int min_dim = (height < width) ? height : width;
    n = (n > min_dim) ? min_dim : n;

    int dh = (int) height / n;
    int mod_h = height % n;

    //int dw = (int) width/n;
    //int mod_w = width % n;
    int x1;

    pthread_t* watki = (pthread_t*) malloc(sizeof(pthread_t) * n);
    struct arg* intervals = (struct arg*) malloc(sizeof(struct arg) * n);

    for(int i = 0; i < n; i++)
    {
        int add_h = (i == n-1) ? mod_h : 0;

        intervals[i].hp = i*dh;
        intervals[i].hk =  (i+1) * dh + add_h - 1;
        intervals[i].wp = 0;
        intervals[i].wk = width-1;
        intervals[i].my_nr = i;

        //printf("i = %d\n", i);
        //printf("%d %d %d %d\n", intervals[i].hp, intervals[i].hk, intervals[i].wp, intervals[i].wk);

        x1 = pthread_create(&watki[i], NULL, (void*) f, &intervals[i]);
        if(x1 == -1){ 
            perror(""); 
        }

    }

    printf("\n");

    sleep(1);
	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); 
	foreground = create_grid(width, height);
	background = create_grid(width, height);

	init_grid(foreground);

    //my_grid(foreground, 4);

    struct sigaction act;
    act.sa_handler = quit_win;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGSTOP, &act, NULL);

	while (true)
	{
		draw_grid(foreground);

        for(int i = 0; i < n; i++)
        {
            pthread_kill(watki[i], SIGUSR1);
        }

		usleep(wait_time * 1000);

		tmp = foreground;
		foreground = background;
		background = tmp;

        //reset_field(background);
	}

	endwin();
	destroy_grid(foreground);
	destroy_grid(background);

	return 0;
}
