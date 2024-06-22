#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <string.h>


char* int_to_string(int num)
{
    int i, rem, len = 0, n;
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    char *str = (char*) malloc(sizeof(char) * (len+1));
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
    return str;
}


char * double_to_string(long double a)
{
    int len = snprintf(NULL, 0, "%.24Lg", a);
    char *result = malloc(len + 1);
    snprintf(result, len + 1, "%.24Lg", a);
    return result;
}


long double f(long double x)
{
    return  4.0/(x*x + 1.0);
}


long double min_ld(long double a, long double b)
{
    if(a > b)
    {
        return b;
    }
    return a;
}


int main(int argc, char* argv[2])
{

    struct timeval stop, start;

    gettimeofday(&start, NULL);

    long double w = strtold(argv[1], NULL);
    long double n = strtold(argv[2], NULL);

    long double step = 1.0/((long double) n);

    FILE * dane_out = fopen("dane.txt", "w");

    printf("\nw = %.11Lg\n", w);
    printf("step = %.11Lg\n", step);

    int** arr = (int**)malloc(n * sizeof(int*));

    for (int i = 0; i < n; i++)
    {
        arr[i] = (int*)malloc(2 * sizeof(int));
    }
    for(int i = 0; i < n; i++)
    {
        int x = pipe(arr[i]);
        if(x == -1)
        {
            exit(2);
        }
    }

    long double global_res = 0.0;

    pid_t ID;
    for(int i = 0; i < n; i++)
    {
        ID = fork();
        if(ID == 0)
        {
            long double xp = i * step;
            long double xk = (i+1) * step;
            long double x_act = xp;
            long double res_i = 0.0;

            while(x_act < xk)
            {
                res_i += w * f(min_ld(x_act + w/2.0, xk)); 
                x_act += w;
            }

            char result[35];
            sprintf(result, "%.24Lg", res_i);
            write(arr[i][1], result, strlen(result));
            break;
        }
        else
        {
            char liczba[35];
            read(arr[i][0], liczba, 28);
            long double partial = strtold(liczba, NULL);
            global_res += partial;
        }
    }

    if(ID != 0)
    {
        
        gettimeofday(&stop, NULL);
        printf("\nOtrzymany wynik: %.24Lg\n", global_res);

        long elapsed = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
        double in_sec = (double) elapsed / 1000000.0;

        printf("Czas wykonania: %lf\n\n", in_sec);

        fwrite("prec: ", 6, 1, dane_out);
        fwrite(argv[1], strlen(argv[1]), 1, dane_out);
        fwrite(" n: ", 4, 1, dane_out);
        fwrite(argv[2], strlen(argv[2]), 1, dane_out);
        
        char wyn[35];

        sprintf(wyn, "%.24Lg", global_res);
        fwrite("\nWynik: ", 8, 1, dane_out);
        fwrite(wyn, strlen(wyn), 1, dane_out);

        fwrite("\nCzas wykonania: ", 17, 1, dane_out);
        char czas[12];
        sprintf(czas, "%lf", in_sec);
        fwrite(czas, strlen(czas), 1, dane_out);

    }
    return 0;
}
