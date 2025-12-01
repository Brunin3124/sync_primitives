#include "sync_lib.h"

// funcoes do mutex
void sync_mutex_init(sync_mutex_t *mutex){
    pthread_mutex_init(&mutex->trava,NULL);
}
void sync_mutex_lock(sync_mutex_t *mutex){
    pthread_mutex_lock(&mutex->trava);
}
void sync_mutex_unlock(sync_mutex_t *mutex){
    pthread_mutex_unlock(&mutex->trava);
}
void sync_mutex_destroy(sync_mutex_t *mutex){
    pthread_mutex_destroy(&mutex->trava);
}