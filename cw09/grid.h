#pragma once
#include <stdbool.h>

struct arg
{
    int hp;
    int hk;
    int wp;
    int wk;
    int my_nr;
};

char *create_grid();
void destroy_grid(char *grid);
void draw_grid(char *grid);
void init_grid(char *grid);
bool is_alive(int row, int col, char *grid);
void update_grid(char *src, char *dst, struct arg * dane);
void my_grid(char * grid, int k);
void print_grid(char * src);
void neighbours(char * src);
bool check_valid(int i, int j);
void reset_field(char * src);