#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "sync_lib.h"


sync_banker_t banco;


#define RECURSO_A 0
#define RECURSO_B 1

void* thread_cliente_A(void* arg) {
    int id = 0; 
    printf("[Thread A] Iniciada. Precisa de A e B.\n");

 
    int req1[] = {1, 0}; 
    printf("[Thread A] Solicitando Recurso A...\n");
    sync_banker_request(&banco, id, req1);
    printf("[Thread A] Conseguiu Recurso A!\n");

    sleep(1); 

  
    int req2[] = {0, 1};
    printf("[Thread A] Solicitando Recurso B...\n");
    sync_banker_request(&banco, id, req2);
    printf("[Thread A] Conseguiu Recurso B! (Zona Crítica Completa)\n");

   
    int release[] = {1, 1};
    sync_banker_release(&banco, id, release);
    printf("[Thread A] Liberou tudo e finalizou.\n");
    return NULL;
}

void* thread_cliente_B(void* arg) {
    int id = 1; 
    printf("[Thread B] Iniciada. Precisa de B e A.\n");

   
    int req1[] = {0, 1}; 
    printf("[Thread B] Solicitando Recurso B...\n");
    sync_banker_request(&banco, id, req1);
    
   
    printf("[Thread B] Conseguiu Recurso B!\n");

    sleep(1);

    // 2. Tenta pegar Recurso A
    int req2[] = {1, 0};
    printf("[Thread B] Solicitando Recurso A...\n");
    sync_banker_request(&banco, id, req2);
    printf("[Thread B] Conseguiu Recurso A! (Zona Crítica Completa)\n");


    int release[] = {1, 1};
    sync_banker_release(&banco, id, release);
    printf("[Thread B] Liberou tudo e finalizou.\n");
    return NULL;
}

int main() {
  
    int recursos_totais[] = {1, 1}; 
    
  
    sync_banker_init(&banco, 2, 2, recursos_totais);

    
    int max_demand[] = {1, 1};
    sync_banker_add_thread(&banco, 0, max_demand);
    sync_banker_add_thread(&banco, 1, max_demand); 

    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread_cliente_A, NULL);
    pthread_create(&t2, NULL, thread_cliente_B, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sync_banker_destroy(&banco);
    
    printf("Teste finalizado com SUCESSO (Sem Deadlock)!\n");
    return 0;
}