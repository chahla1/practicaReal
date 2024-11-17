#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define SIZE 10

//Definimos primero la estructura
typedef struct {
    int base;
    int exp;
    int potencia;
} potencia_t;


