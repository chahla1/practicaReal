/* hebConcurrente.c */
#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#define DIMFILA 300000
#define NUMFILAS 20

typedef struct fila{
    int vector[DIMFILA];
    long suma;
} fila_t;

fila_t matriz[NUMFILAS];

typedef struct {
    int idx;
} arg_t;

void *worker(void *a){
    arg_t *ag = (arg_t*)a;
    int i = ag->idx;
    long s = 0;
    for(int j=0;j<DIMFILA;j++) s += matriz[i].vector[j];
    matriz[i].suma = s;
    free(ag);
    return NULL;
}

int main(int argc, char **argv){
    pthread_t ths[NUMFILAS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);


    if(argc > 1){
        int scope = (strcmp(argv[1],"system")==0) ? PTHREAD_SCOPE_SYSTEM : PTHREAD_SCOPE_PROCESS;
        if(pthread_attr_setscope(&attr, scope) != 0){
            fprintf(stderr, "Aviso: pthread_attr_setscope failed (no soportado). Continuando.\n");
        } else {
            int read_scope;
            pthread_attr_getscope(&attr, &read_scope);
            if(read_scope==PTHREAD_SCOPE_SYSTEM) printf("Ámbito: PTHREAD_SCOPE_SYSTEM\n");
            else printf("Ámbito: PTHREAD_SCOPE_PROCESS\n");
        }
    }

    /* Inicializar matriz */
    for(int i=0;i<NUMFILAS;i++){
        matriz[i].suma = 0;
        for(int j=0;j<DIMFILA;j++) matriz[i].vector[j] = 10;
    }

    /* Crear hebras */
    for(int i=0;i<NUMFILAS;i++){
        arg_t *a = malloc(sizeof(arg_t));
        a->idx = i;
        if(pthread_create(&ths[i], &attr, worker, a) != 0){
            perror("pthread_create");
            exit(1);
        }
    }

    for(int i=0;i<NUMFILAS;i++){
        pthread_join(ths[i], NULL);
    }

    for(int i=0;i<NUMFILAS;i++){
        printf("Fila %2d: suma = %ld\n", i, matriz[i].suma);
    }

    pthread_attr_destroy(&attr);
    return 0;
}
