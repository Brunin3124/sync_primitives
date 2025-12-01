#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "sync_lib.h"

long long count = 0;
sync_mutex_t mutex_count;

#define ITERATIONS 10000 

void *incrementar(void *arg) {
    for (int i = 0; i < ITERATIONS; i++) {

        sync_mutex_lock(&mutex_count);

        count = count + 1;

        sync_mutex_unlock(&mutex_count);

    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    sync_mutex_init(&mutex_count);

    if (pthread_create(&thread1, NULL, incrementar, NULL) != 0) {
        perror("Erro ao criar thread 1");
        return 1;
    }
    if (pthread_create(&thread2, NULL, incrementar, NULL) != 0) {
        perror("Erro ao criar thread 2");
        return 1;
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    sync_mutex_destroy(&mutex_count);

    printf("Resultado obtido = %lld\n", count);

    return 0;
}