#include "sync_lib.h"



void sync_cond_init(sync_cond_t *cond){
    pthread_cond_init(&cond->cond, NULL);
}

void sync_cond_wait(sync_cond_t *cond, sync_mutex_t *mutex){
    pthread_cond_wait(&cond->cond, &mutex->trava);
}

void sync_cond_signal(sync_cond_t *cond){
    pthread_cond_signal(&cond->cond);
}

void sync_cond_broadcast(sync_cond_t *cond){
    pthread_cond_broadcast(&cond->cond);
}

void sync_cond_destroy(sync_cond_t *cond){
    pthread_cond_destroy(&cond->cond);
}