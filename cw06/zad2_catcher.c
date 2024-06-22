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

long double min_ld(long double a, long double b)
{
    if(a > b)
    {
        return b;
    }
    return a;
}

long double get(FILE* in, char sign)
{
    char * napis = "\0";
    char c[1];
    int count = 0;
    while(1)
    {
        count++;
        fread(&c, 1, 1, in);
        if(c[0] == sign)
        {
            break;
        }
        napis = concat_strings(napis, c);
    }
    long double res = strtold(napis, NULL);
    return res;
}

long double func(long double x)
{
    return 4.0/(1+x*x);
}
long double integral(long double p1, long double p2, long double step, long double (*f)(long double))
{
    long double act = p1, res = 0.0;
    while(act < p2)
    {
        res += step * f( min_ld(act + step/2.0, p2) );
        act += step;
    }
    return res;
}


int main()
{
    FILE * in = fopen("potok1", "r");


    long double p1 = get(in, '\n');
    long double p2 = get(in, '\n');
    long double step = get(in, '\n');
    fclose(in);


    printf("\nWczytano:\np1 = %.10Lg\n", p1);
    printf("p2 = %.10Lg\n", p2);
    printf("step = %.10Lg\n\n", step);


    long double wyn = integral(p1, p2, step, *func);
    char jd[35];
    printf("Obliczono:\nwyn = %.28Lg\n", wyn);
    sprintf(jd, "%.20Lg", wyn);
    printf("str(wyn) = %s\nsize = %ld\n\nKoncze catcher\n\n", jd, strlen(jd));

    
    FILE * out = fopen("potok1", "w");
    fwrite(jd, strlen(jd), 1, out);
    fwrite("\n", 1, 1, out);
    fclose(out);

    
    return 0;
}