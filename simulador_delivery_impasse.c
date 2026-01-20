#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_RESTARANTES 1
#define NUM_ENTREGADORES 2

pthread_mutex_t moto = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t pedido = PTHREAD_MUTEX_INITIALIZER;

// Tipo 1: O veterano (Moto -> Pedido)
void *entregador_veterano(void *arg) {
    printf("[Veterano]: Tentando pegar a chave da MOTO ..\n");
    pthread_mutex_lock(&moto); // Pega a moto
    printf("[Veterano]: Consegui a MOTO! Indo buscar o lanche ..\n");

    sleep(2); // Caminhada (tempo para o Novato pegar o pedido)

    printf("[Veterano]: Tentando pegar o PEDIDO ..\n");
    pthread_mutex_lock(&pedido); // Vai travar aqui! 

    printf("[Veterano]: Consegui tudo! Saindo para entrega.\n");
    pthread_mutex_unlock(&pedido);
    pthread_mutex_unlock(&moto);

    return NULL;
}

// Tipo 2: O novato (Pedido -> Moto)
void *entregador_novato(void *arg) {
    printf("[Novato]: Tentando pegar o PEDIDO ..\n");
    pthread_mutex_lock(&pedido); // Pega o pedido 
    printf("[Novato]: Consegui o PEDIDO! Indo buscar a moto ..\n");

    sleep(2); // Caminhada (tempo para o Veterano pegar a moto)

    printf("[Novato]: Tentando pegar a MOTO ..\n");
    pthread_mutex_lock(&moto); // Vai travar aqui!

    printf("[Novato]: Consegui tudo! Partiu entrega.\n");
    pthread_mutex_unlock(&moto);
    pthread_mutex_unlock(&pedido);

    return NULL;
}

int main () {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, entregador_veterano, NULL);
    pthread_create(&t2, NULL, entregador_novato, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}