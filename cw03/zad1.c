#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <string.h>
#include <stdlib.h>

void rev(char * napis, int n)
{
    for(int i = 0; i < n/2; i++)
    {
        char temp = napis[i];
        napis[i] = napis[n-1-i];
        napis[n-i-1] = temp;
    }
}

char* slice(char * napis, int a, int b)
{   
    char *new_string = (char*) malloc(sizeof(char) * (b-a + 2));
    for(int i = 0; i < (b-a+1); i++)
    {
        new_string[i] = napis[i+a];
    }
    new_string[b-a+1] = '\0';
    return new_string;
}


void read_reverse(char * in, char * out, int start_size)
{
    char * c = (char*) malloc(sizeof(char) * (start_size+5));
    int block_size = start_size;
    FILE * we = fopen(in, "r+");
    FILE *wy1 = fopen(out, "w+");
    fseek(we, 0, SEEK_END);
    int f = fseek(we, -block_size, SEEK_CUR);

    if(f != 0)
    {
        block_size = 1;
        fseek(we, -1, SEEK_CUR);
    }

    while(1)
    {
        fread(c, block_size, 1, we);
        rev(c, block_size);
        fwrite(c, block_size, 1, wy1);
        int val = fseek(we, -2*block_size, SEEK_CUR);
        if(val != 0)
        {
            if(block_size == start_size && block_size != 1)
            {
                int y = fseek(we, -1-block_size, SEEK_CUR);
                if(y != 0){
                    break;
                }
                block_size = 1;
            }
            else
                break;
        }
    }
    fclose(we);
    fclose(wy1);
}


int main()
{
    read_reverse("input.txt", "output1024.txt", 1024);
    read_reverse("input.txt", "output1.txt", 1);
    read_reverse("output1024.txt", "output1024_reversed.txt", 1024);
    read_reverse("output1.txt", "output1_reversed.txt", 1);
    return 0;
}