#include "sync_lib.h"

// funcoes do mutex
void mutex_init(sync_mutex_t *mutex){
    pthread_mutex_init(&mutex->trava,NULL);
}
void mutex_lock(sync_mutex_t *mutex){
    pthread_mutex_lock(&mutex->trava);
}
void mutex_unlock(sync_mutex_t *mutex){
    pthread_mutex_unlock(&mutex->trava);
}
void mutex_destroy(sync_mutex_t *mutex){
    pthread_mutex_destroy(&mutex->trava)
}