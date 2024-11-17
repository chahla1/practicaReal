
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 10

//Primero definimos la estructura
typedef struct {
    int base;
    int exp;
    int potencia;
} potencia_t;

typedef potencia_t *potenciaP_t;

// Ahora definimos las funciones basicas
void setBaseExp(potenciaP_t pot, int base, int exp) {
    pot->base = base;
    pot->exp = exp;
    pot->potencia = -1;
}

int getPotencia(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

void setPotenciaEst(potenciaP_t pot) {
    pot->potencia = getPotencia(pot->base, pot->exp);
}

void initArrayEst(potencia_t array[]) {
    for (int i = 0; i < SIZE; i++) {
        array[i].base = i + 1;
        array[i].exp = 0;
        array[i].potencia = 1;
    }
}

void printArrayEst(potencia_t array[]) {
    for (int i = 0; i < SIZE; i++) {
        printf("arr[%d]: base: %d exp: %d potencia: %d\n", i, array[i].base, array[i].exp, array[i].potencia);
    }
}

// Funcion para hebras.
void *calcuPotHeb(void *arg) {
    potencia_t *pot = (potencia_t *)arg;
    pot->potencia = getPotencia(pot->base, pot->exp);
    pthread_exit(NULL);
}

int main() {
    potencia_t arr1[SIZE];
    pthread_t threads[SIZE];

    // Iniciamos el array.
    initArrayEst(arr1);
    printArrayEst(arr1);

    // Modificamos el array.
    for (int i = 0; i < SIZE; i++) {
        setBaseExp(&arr1[i], i, 2);
    }
    printArrayEst(arr1);

    // Creamos hebras para calcular las potencias.
    for (int i = 0; i < SIZE; i++) {
        pthread_create(&threads[i], NULL, calcuPotHeb, (void *)&arr1[i]);
    }


    for (int i = 0; i < SIZE; i++) {
        pthread_join(threads[i], NULL);
    }

    printArrayEst(arr1);

    return 0;
}

