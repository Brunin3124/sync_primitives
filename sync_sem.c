#include "sync_lib.h"


void sync_sem_init(*sync_sem_t semaforo, int valor){
    sem_init(&semaforo->sem,0,valor);
}
void sync_sem_wait(*sync_sem_t semaforo){
    sem_wait(&semaforo->sem);
}
void sync_sem_post(*sync_sem_t semaforo){
    sem_post(&semaforo->sem);
}
void sync_sem_destroy(*sync_sem_t semaforo){
    sem_destroy(&semaforo);
}