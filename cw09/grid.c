#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "grid.h"

int grid_width = 0;
int grid_height = 0;



char *create_grid(int w, int h)
{
    grid_width = w;
    grid_height = h;
    return malloc(sizeof(char) * w * h);
}

void destroy_grid(char *grid)
{
    free(grid);
}

bool check_valid(int i, int j)
{
    return (i >= 0 && i < grid_height && j >= 0 && j < grid_width);
}

void draw_grid(char *grid)
{
    for (int i = 0; i < grid_height; ++i)
    {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < grid_width; ++j)
        {
            if (grid[i * grid_width + j])
            {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else
            {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid)
{
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = rand() % 2 == 0;
}

void my_grid(char * grid, int k)
{
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = ((i % k) == 0);
}
bool is_alive(int row, int col, char *grid)
{

    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            int r = row + i;
            int c = col + j;

            if (check_valid(r, c) && grid[grid_width * r + c])
            {
                count++;
            }
        }
    }

    if (grid[row * grid_width + col])
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if (count == 3)
            return true;
        else
            return false;
    }
}

void update_grid(char *src, char *dst, struct arg * dane)
{
    //printf("%d %d %d %d\n", dane->hp, dane->hk, dane->wp, dane->wk);

    for (int i = dane->hp; i <= dane->hk; ++i)
    {
        for (int j = dane->wp; j <= dane->wk; ++j)
        {
            dst[i * grid_width + j] = is_alive(i, j, src);
        }
    }
}

void print_grid(char * src)
{
    for(int i = 0; i < grid_height; i++)
    {
        for(int j = 0; j < grid_width; j++)
        {
            printf("%d ", src[i * grid_width + j]);
        }
        printf("\n");
    }
}


void neighbours(char * src)
{
    for(int i = 0 ; i < grid_height; i++)
    {
        for(int j = 0; j < grid_width; j++)
        {
            int count = 0;
            for(int di = -1; di <= 1; di++)
            {
                for(int dj = -1; dj <= 1; dj++)
                {
                    if(di != 0 || dj != 0)
                    {
                        if(check_valid(i+di, j+dj))
                        {
                            count += src[(i+di) * grid_width + (j + dj)];
                        }
                    }
                }
            }
            printf("%d ", count);
        }
        printf("\n");
    }
}

void reset_field(char * src)
{
    for(int i = 0 ; i < grid_height; i++)
    {
        for(int j = 0; j < grid_width; j++)
        {
            src[i * grid_width + j] = 0;
        }
    }
}

