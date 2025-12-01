#include <stdio.h>
#include <time.h>
#include "sync_lib.h"

/* Protótipos exportados por perfomance_test.c */
void contagem_serial(void);
void rodar_teste_paralelo(void);

/* Variáveis globais definidas em perfomance_test.c (declaradas externamente
   para que possamos verificar resultados nos testes). */
extern long cont_primo_serial;
extern long cont_primo_paralelo;

static double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

static void run_and_measure(const char *name, void (*test_fn)(void), long *out_count) {
    double t0 = now_seconds();
    test_fn();
    double t1 = now_seconds();
    printf("%s tempo: %.4f s — primos: %ld\n", name, t1 - t0, *out_count);
}

int main(void) {
    printf("== Teste de performance: serial vs paralelo ==\n");

   
    run_and_measure("serial: ", contagem_serial, &cont_primo_serial);

   
    run_and_measure("paralelo: ", rodar_teste_paralelo, &cont_primo_paralelo);

    
    if (cont_primo_serial == cont_primo_paralelo) {
        printf("Resultado: contagens iguais (%ld)\n", cont_primo_serial);
        return 0;
    } else {
        printf("Resultado: FALHA — serial=%ld, paralelo=%ld\n", cont_primo_serial, cont_primo_paralelo);
        return 2;
    }
}