#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "sync_lib.h"


#define LIMITE_SUPERIOR 3000000 
#define NUM_THREADS 4           


long total_primos_serial = 0;
long total_primos_paralelo = 0;
sync_mutex_t meu_mutex;


int eh_primo(int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}


double pegar_tempo() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}


void rodar_teste_serial() {
    printf("[SERIAL] Iniciando contagem de 0 a %d...\n", LIMITE_SUPERIOR);
    total_primos_serial = 0;
    
    double inicio = pegar_tempo();
    
    for (int i = 0; i < LIMITE_SUPERIOR; i++) {
        if (eh_primo(i)) {
            total_primos_serial++;
        }
    }
    
    double fim = pegar_tempo();
    printf("[SERIAL] Concluido em: %.4f segundos. Primos: %ld\n", fim - inicio, total_primos_serial);
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

    // zona do perigo
    sync_mutex_lock(&meu_mutex);
    total_primos_paralelo += contagem_local;
    sync_mutex_unlock(&meu_mutex);

    free(range);
    return NULL;
}

void rodar_teste_paralelo() {
    printf("\n[PARALELO] Iniciando com %d threads usando sync_mutex...\n", NUM_THREADS);
    total_primos_paralelo = 0;
    sync_mutex_init(&meu_mutex); 

    pthread_t threads[NUM_THREADS];
    double inicio = pegar_tempo();

    int tamanho_fatia = LIMITE_SUPERIOR / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        intervalo_t* range = malloc(sizeof(intervalo_t));
        range->inicio = i * tamanho_fatia;
        
    
        if (i == NUM_THREADS - 1) {
            range->fim = LIMITE_SUPERIOR;
        } else {
            range->fim = (i + 1) * tamanho_fatia;
        }

        pthread_create(&threads[i], NULL, worker_primos, range);
    }

        //finzaliando thread
        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);
        pthread_join(threads[2], NULL);
        pthread_join(threads[3], NULL);
    

    double fim = pegar_tempo();
    sync_mutex_destroy(&meu_mutex); 

    printf("[PARALELO] Concluido em: %.4f segundos. Primos: %ld\n", fim - inicio, total_primos_paralelo);
    
   
    return;
}


int main() {
    printf("=== Teste de desempenho ===\n");
    
    // 1. Roda Serial
    double t_inicio_s = pegar_tempo();
    rodar_teste_serial();
    double t_fim_s = pegar_tempo();
    double tempo_serial = t_fim_s - t_inicio_s;

    // 2. Roda Paralelo
    double t_inicio_p = pegar_tempo();
    rodar_teste_paralelo();
    double t_fim_p = pegar_tempo();
    double tempo_paralelo = t_fim_p - t_inicio_p;

    // 3. Resultados
    printf("\n=== RESULTADO FINAL ===\n");
    if (total_primos_serial == total_primos_paralelo) {
        printf("VALIDACAO: SUCESSO (Resultados iguais: %ld)\n", total_primos_serial);
    } else {
        printf("VALIDACAO: FALHA (Serial: %ld vs Paralelo: %ld)\n", total_primos_serial, total_primos_paralelo);
    }

    double speedup = tempo_serial / tempo_paralelo;
    printf("velocidade obtida: %.2fx mais rapido\n", speedup);

    return 0;
}