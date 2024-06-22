#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h> 
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

char * conc(char * str1, char* str2)
{
    int n = strlen(str1);
    int m = strlen(str2);
    char * res = (char*) malloc((n + m + 1) * sizeof(char));
    for(int i = 0; i < n; i++){
        res[i] = str1[i];
    }
    for(int i = 0; i < m; i++){
        res[n+i] = str2[i];
    }

    res[n+m] = '\0';
    return res;
}

int main()
{
    char * dir = "./";
    DIR * katalog = opendir(dir);
    if(katalog == NULL)
    {
        fprintf(stderr, "Can't open %s\n", dir);
        return -10;
    }

    struct dirent *dp;
    char* filename;
    printf("\n");
    struct stat st;
    long long total_size = 0;
    while ((dp = readdir(katalog)) != NULL) 
    {
        filename = dp->d_name;
        char * sciezka = conc("./", filename);
        int x = stat(sciezka, &st);

        if(x == -1){
            printf("Nie mozna uzyskac danych na temat pliku: %s\n", filename);
            return -10;
        }

        if(!S_ISDIR(st.st_mode))
        {
            printf("plik: %s, ", filename);
            printf("rozmiar: %ld\n", st.st_size);
            total_size += st.st_size;
        }
    }
    printf("\nCalkowity rozmiar: %lld\n\n", total_size);
    closedir(katalog);
    return 0;
}