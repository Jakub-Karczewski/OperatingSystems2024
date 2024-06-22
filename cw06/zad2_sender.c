#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <string.h>

char * concat_strings(char* s1, char*s2)
{
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    char * s3 = (char*) malloc(sizeof(char) * (l1+l2+1));
    for(int i = 0; i < l1; i++)
    {
        s3[i] = s1[i];
    }
    for(int j = 0; j < l2; j++){
        s3[l1+j] = s2[j];
    }
    s3[l1+l2] = '\0';
    return s3;
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

int main()
{

    char s1[35], s2[35], s3[35];
    long double p1, p2, step;

    printf("\nPodaj (a, b) oraz step\n");
    scanf("%Lf", &p1);
    scanf("%Lf", &p2);
    scanf("%Lf", &step);


    sprintf(s1, "%.20Lg", p1);
    sprintf(s2, "%.20Lg", p2);
    sprintf(s3, "%.20Lg", step);


    FILE * out = fopen("potok1", "w");

    fwrite(s1, strlen(s1), 1, out);
    fwrite("\n", 1, 1, out);
    fwrite(s2, strlen(s2), 1, out);
    fwrite("\n", 1, 1, out);
    fwrite(s3, strlen(s3), 1, out);
    fwrite("\n", 1, 1, out);
    fclose(out);

    
    char c[2];
    char* napis = "\0";

    FILE * in = fopen("potok1", "r");

    while(1)
    {
        fread(&c, 1, 1, in);
        char* ss = slice(c, 0, 0);
        if(c[0] == '\n')
        {
            break;
        }
        napis = concat_strings(napis, ss);
    }

    printf("\nOtrzymano wynik:\n%s\n\nKoncze sender\n\n", napis);
    fclose(in);
    
    return 0;
}