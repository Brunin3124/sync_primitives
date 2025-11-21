#ifndef SYNC_LIB_H
#define SYNC_LIB_H

#include <pthread.h>


//struct do mutex
typedef struct{
    pthread_mutex_t trava;
} sync_mutex_t;
//funcoes do mutex
void sync_mutex_init(sync_mutex_t *mutex);
void sync_mutex_lock(sync_mutex_t *mutex);
void sync_mutex_unlock(sync_mutex_t *mutex);
void sync_mutex_destroy(sync_mutex_t *mutex);

//struct do semaforo
typedef struct{
    sem_t sem;
}sync_sem_t;

void sync_sem_init(sync_sem_t *semaforo, int valor);
void sync_sem_wait(sync_sem_t *semaforo);
void sync_sem_post(sync_sem_t *semaforo);
void sync_sem_destroy(sync_sem_t *semaforo);
#endif