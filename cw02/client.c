#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
int main(){
    void *plik = dlopen("./libcollatz_shared.so", RTLD_LAZY);
    if(!plik){
        return(-69);
    }

    int (*collatz_conjecture)(int input);
    int (*test_collatz_convergence)(int input, int max_iter);
    collatz_conjecture = dlsym(plik, "collatz_conjecture");
    test_collatz_convergence = dlsym(plik, "test_collatz_convergence");
    if(dlerror == NULL){
        return (-69);
    }
    int inp = 2048;
    int iter = 15;
    printf("Input: %d, Max_iter: %d\n", inp, iter);
    printf("Kolejna iteracja: %d\n", (*collatz_conjecture)(inp) );
    printf("Liczba wykonanych iteracji: %d\n", (*test_collatz_convergence)(inp, iter) );
    printf("DONE!!\n");
    dlclose(plik);
}