/* hebSecuencial.c */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define DIMFILA 300000
#define NUMFILAS 20

typedef struct fila{
    int vector[DIMFILA];
    long suma;
} fila_t;

fila_t matriz[NUMFILAS];

void *trabajo(void *arg){
    int i,j;
    for(i=0;i<NUMFILAS;i++){
        matriz[i].suma = 0;
        for(j=0;j<DIMFILA;j++){
            matriz[i].vector[j] = 10;
        }
    }
    for(i=0;i<NUMFILAS;i++){
        long s = 0;
        for(j=0;j<DIMFILA;j++){
            s += matriz[i].vector[j];
        }
        matriz[i].suma = s;
    }
    return NULL;
}

int main(){
    pthread_t th;
    if(pthread_create(&th, NULL, trabajo, NULL) != 0){
        perror("pthread_create");
        exit(1);
    }
    pthread_join(th, NULL);

    for(int i=0;i<NUMFILAS;i++){
        printf("Fila %2d: suma = %ld\n", i, matriz[i].suma);
    }
    return 0;
}
