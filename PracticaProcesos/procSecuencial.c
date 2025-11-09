/* procSecuencial.c */
#include <stdio.h>
#include <stdlib.h>

#define DIMFILA 300000
#define NUMFILAS 20

typedef struct fila{
    int vector[DIMFILA];
    long suma;
} fila_t;

fila_t matriz[NUMFILAS]; /* matriz global para no usar stack */

int main(){
    int i, j;

    /* Inicializar */
    for(i=0;i<NUMFILAS;i++){
        matriz[i].suma = 0;
        for(j=0;j<DIMFILA;j++){
            matriz[i].vector[j] = 10;
        }
    }

    /* Procesado secuencial */
    for(i=0;i<NUMFILAS;i++){
        long s = 0;
        for(j=0;j<DIMFILA;j++){
            s += matriz[i].vector[j];
        }
        matriz[i].suma = s;
    }

    /* Mostrar resultados */
    for(i=0;i<NUMFILAS;i++){
        printf("Fila %2d: suma = %ld\n", i, matriz[i].suma);
    }

    return 0;
}
