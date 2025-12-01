#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long long count = 0;

#define ITERATIONS 10000 

void *incrementar(void *arg) {
    for (int i = 0; i < ITERATIONS; i++) {

        count = count + 1;

    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

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

    printf("Resultado obtido = %lld\n", count);

    return 0;
}