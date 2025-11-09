/* procConcurrente.c */
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define DIMFILA 300000
#define NUMFILAS 20

typedef struct fila {
    int vector[DIMFILA];
    long suma;
} fila_t;

void calculate_row(int idx, fila_t *mat) {
    long s = 0;
    for (int j = 0; j < DIMFILA; j++) {
        s += mat[idx].vector[j];
    }
    mat[idx].suma = s;
}

int main() {
    const char *fname = "matriz_shm.tmp";
    HANDLE hMapFile;
    fila_t *mat;

    size_t size = sizeof(fila_t) * NUMFILAS;


    hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, (DWORD)size, fname);
    if (hMapFile == NULL) {
        fprintf(stderr, "CreateFileMapping failed (%d)\n", GetLastError());
        exit(1);
    }


    mat = (fila_t *)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, size);
    if (mat == NULL) {
        fprintf(stderr, "MapViewOfFile failed (%d)\n", GetLastError());
        CloseHandle(hMapFile);
        exit(1);
    }

    /* inicializamos la matrix */
    for (int i = 0; i < NUMFILAS; i++) {
        mat[i].suma = 0;
        for (int j = 0; j < DIMFILA; j++) {
            mat[i].vector[j] = 10;
        }
    }


    HANDLE processes[NUMFILAS];
    for (int i = 0; i < NUMFILAS; i++) {
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "%s %d", fname, i);

        STARTUPINFO si = { sizeof(si) };
        PROCESS_INFORMATION pi;

        if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            fprintf(stderr, "CreateProcess failed (%d)\n", GetLastError());
            UnmapViewOfFile(mat);
            CloseHandle(hMapFile);
            exit(1);
        }

        processes[i] = pi.hProcess;
        CloseHandle(pi.hThread);
    }

    /* esperamos que todos los procesos terminen */
    WaitForMultipleObjects(NUMFILAS, processes, TRUE, INFINITE);


    for (int i = 0; i < NUMFILAS; i++) {
        CloseHandle(processes[i]);
    }


    for (int i = 0; i < NUMFILAS; i++) {
        printf("Fila %2d: suma = %ld\n", i, mat[i].suma);
    }


    UnmapViewOfFile(mat);
    CloseHandle(hMapFile);

    return 0;
}