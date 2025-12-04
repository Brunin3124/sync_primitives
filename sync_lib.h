#ifndef SYNC_LIB_H
#define SYNC_LIB_H

#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>


//struct do mutex
typedef struct{
    pthread_mutex_t trava;
} sync_mutex_t;
//funcoes do mutex
void sync_mutex_init(sync_mutex_t *mutex);
void sync_mutex_lock(sync_mutex_t *mutex);
void sync_mutex_unlock(sync_mutex_t *mutex);
void sync_mutex_destroy(sync_mutex_t *mutex);

// struct do semaforo
typedef struct {
    sem_t sem;
} sync_sem_t;

void sync_sem_init(sync_sem_t *semaforo, int valor);
void sync_sem_wait(sync_sem_t *semaforo);
void sync_sem_post(sync_sem_t *semaforo);
void sync_sem_destroy(sync_sem_t *semaforo);

//struct cond_var
typedef struct{
    pthread_cond_t cond;
} sync_cond_t;

//funcoes
void sync_cond_init(sync_cond_t *cond);
void sync_cond_wait(sync_cond_t *cond, sync_mutex_t *mutex);
void sync_cond_signal(sync_cond_t *cond);
void sync_cond_broadcast(sync_cond_t *cond);
void sync_cond_destroy(sync_cond_t *cond);

//struct do banqueiro
typedef struct {
    int *available;      
    int **max;          
    int **allocation;    
    int **need;         
    
    int resources;   
    int threads;     
    
    
    sync_mutex_t lock;   
    sync_cond_t cond;   
} sync_banker_t;

//funça do banqueiro
void sync_banker_init(sync_banker_t *b, int threads, int resources, int *resources_initiais);
int sync_banker_add_thread(sync_banker_t *b, int thread_id, int *maximo);
void sync_banker_request(sync_banker_t *b, int thread_id, int *request);
void sync_banker_release(sync_banker_t *b, int thread_id, int *release);
void sync_banker_destroy(sync_banker_t *b);


#endif