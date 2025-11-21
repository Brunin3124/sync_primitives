#ifndef SYNC_MUTEX_H
#define SYNC_MUTEX_H

#include <pthread.h>


//struct
typedef struct{
    pthread_mutex_t lock;
} sync_mutex_t;

void mutex_init(sync_mutex_t *mutex);
void mutex_lock(sync_mutex_t *mutex);
void mutex_unlock(sync_mutex_t *mutex);
void mutex_destroy(sync_mutex_t *mutex);

#endif