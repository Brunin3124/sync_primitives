#ifndef SYNC_LIB_H
#define SYNC_LIB_H

#include <pthread.h>


//struct do mutex
typedef struct{
    pthread_mutex_t trava;
} sync_mutex_t;
//funcoes do mutex
void mutex_init(sync_mutex_t *mutex);
void mutex_lock(sync_mutex_t *mutex);
void mutex_unlock(sync_mutex_t *mutex);
void mutex_destroy(sync_mutex_t *mutex);

//struct do semaforo
#endif