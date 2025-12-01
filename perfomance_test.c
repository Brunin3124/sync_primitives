#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "sync_lib.h"
#include <stdio.h>

#define LIMITE_SUPERIOR 3000000
#define NUM_THREADS 4

long cont_primo_serial = 0;
long cont_primo_paralelo = 0;
sync_mutex_t mutex;

static double pegar_tempo(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int eh_primo(int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

void contagem_serial(){
    cont_primo_serial = 0;
    for(int i = 0; i < LIMITE_SUPERIOR; i++){
        if(eh_primo(i)){
            cont_primo_serial++;
        }
    }
    printf("%ld\n", cont_primo_serial);
}

typedef struct {
    int inicio;
    int fim;
} intervalo_t;

void* worker_primos(void* arg) {
    intervalo_t* range = (intervalo_t*)arg;
    long contagem_local = 0; 

    for (int i = range->inicio; i < range->fim; i++) {
        if (eh_primo(i)) {
            contagem_local++;
        }
    }

    
    sync_mutex_lock(&mutex);
    cont_primo_paralelo += contagem_local;
    sync_mutex_unlock(&mutex);

    free(range);
    return NULL;
}

void rodar_teste_paralelo() {
    printf("\n PARALELO:  Iniciando com %d threads usando sync_mutex...\n", NUM_THREADS);
    cont_primo_paralelo = 0;
    sync_mutex_init(&mutex); 

    pthread_t threads[NUM_THREADS];
    double inicio = pegar_tempo();

    int tamanho_fatia = LIMITE_SUPERIOR / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        intervalo_t* range = malloc(sizeof(intervalo_t));
        range->inicio = i * tamanho_fatia;
        
        // Garante que a última thread pegue até o finalzinho (se sobrar resto)
        if (i == NUM_THREADS - 1) {
            range->fim = LIMITE_SUPERIOR;
        } else {
            range->fim = (i + 1) * tamanho_fatia;
        }

        pthread_create(&threads[i], NULL, worker_primos, range);
    }

    // Espera todas terminarem
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    double fim = pegar_tempo();
    sync_mutex_destroy(&mutex); // Sua função destroy

    printf("PARALELO Concluido em: %.4f segundos. Primos: %ld\n", fim - inicio, cont_primo_paralelo);
    
  
    return;
}