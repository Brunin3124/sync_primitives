#include "sync_lib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 


int _is_safe(sync_banker_t *b) {
    int work[b->resources];
    int finish[b->threads];
    
  
    for(int i=0; i<b->resources; i++){
        work[i] = b->available[i]; // work recebe qnt recursos disponiveis
    } 
  
    for(int i=0; i<b->threads; i++){
        finish[i] = 0; //threads recebem zero pq no inicio nenhuma terminou a tarefa
    } 

    int count = 0;
    while (count < b->threads) {
        int found = 0;
        for (int p = 0; p < b->threads; p++) {
            if (finish[p] == 0) {
                int j;
                for (j = 0; j < b->resources; j++)
                    if (b->need[p][j] > work[j]) break;

                if (j == b->resources) {
                    for (int k = 0; k < b->resources; k++)
                        work[k] += b->allocation[p][k];
                    finish[p] = 1;
                    found = 1;
                    count++;
                }
            }
        }
        if (found == 0) return 0; 
    }
    return 1; 
}

void sync_banker_init(sync_banker_t *b, int threads, int resources, int *resources_initiais) {
    b->threads = threads;
    b->resources = resources;

   
    b->available = (int*) malloc(resources * sizeof(int));
    for(int i=0; i<resources; i++) b->available[i] = resources_initiais[i];

    b->max = (int**) malloc(threads * sizeof(int*));
    b->allocation = (int**) malloc(threads * sizeof(int*));
    b->need = (int**) malloc(threads * sizeof(int*));

    for(int i=0; i<threads; i++) {
        b->max[i] = (int*) calloc(resources, sizeof(int));
        b->allocation[i] = (int*) calloc(resources, sizeof(int));
        b->need[i] = (int*) calloc(resources, sizeof(int));
    }

  
    sync_mutex_init(&b->lock);
    sync_cond_init(&b->cond);
}

int sync_banker_add_thread(sync_banker_t *b, int thread_id, int *maximo) {
    sync_mutex_lock(&b->lock);
    for(int i=0; i<b->resources; i++) {
        b->max[thread_id][i] = maximo[i];
        b->need[thread_id][i] = maximo[i];
    }
    sync_mutex_unlock(&b->lock);
    return 0;
}

void sync_banker_request(sync_banker_t *b, int thread_id, int *request) {
    sync_mutex_lock(&b->lock);

    while (1) {
       
        int possivel = 1;
        for(int i=0; i<b->resources; i++) {
            if (request[i] > b->available[i]) {
                possivel = 0;
                break;
            }
        }

        if (!possivel) {
        
            sync_cond_wait(&b->cond, &b->lock);
            continue;
        }

    
        for(int i=0; i<b->resources; i++) {
            b->available[i] -= request[i];
            b->allocation[thread_id][i] += request[i];
            b->need[thread_id][i] -= request[i];
        }

      
        if (_is_safe(b)) {
         
            sync_mutex_unlock(&b->lock);
            return;
        } else {
     
            for(int i=0; i<b->resources; i++) {
                b->available[i] += request[i];
                b->allocation[thread_id][i] -= request[i];
                b->need[thread_id][i] += request[i];
            }
         
            sync_cond_wait(&b->cond, &b->lock);
        }
    }
}

void sync_banker_release(sync_banker_t *b, int thread_id, int *release) {
    sync_mutex_lock(&b->lock);

    for(int i=0; i<b->resources; i++) {
        b->allocation[thread_id][i] -= release[i];
        b->available[i] += release[i];
        b->need[thread_id][i] += release[i];
    }


    sync_cond_broadcast(&b->cond);
    sync_mutex_unlock(&b->lock);
}

void sync_banker_destroy(sync_banker_t *b) {
    free(b->available);
    for(int i=0; i<b->threads; i++) {
        free(b->max[i]);
        free(b->allocation[i]);
        free(b->need[i]);
    }
    free(b->max);
    free(b->allocation);
    free(b->need);

    sync_mutex_destroy(&b->lock);
    sync_cond_destroy(&b->cond);
}