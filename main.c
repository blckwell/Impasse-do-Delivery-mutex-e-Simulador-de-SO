#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>

#define NUM_RESTAURANTES 5
#define NUM_ENTREGADORES 6 // Mais entregadores do que restaurantes para forçar o bug

// Arrays de mutexes (as "chaves")
pthread_mutex_t motos[NUM_RESTAURANTES];
pthread_mutex_t pedidos[NUM_RESTAURANTES];

void *entregador_veterano (void *arg) {
    int id = *(int *)arg;
    int r = rand() % NUM_RESTAURANTES; // Escolhe um restaurante alvo

    printf("[Veterano %d]: Indo para o Restaurante %d\n", id, r);

    // 1. Bloqueia a Moto 
    pthread_mutex_lock (&motos[r]);
    printf("[Veterano %d]: Peguei a chave da moto do Restaurante %d\n", id, r);

    sleep(1); // Tempo de caminhada até o balcão 

    // 2. Tenta pegar o Pedido
    printf("[Veterano %d]: Aguardando lanche do Restaurante %d ..\n", id, r);
    pthread_mutex_lock(&pedidos[r]);
    
    // Se chegar aqui, conseguiu ambos!
    printf("[Veterano %d]: Entrega realizada!\n", id);

    pthread_mutex_unlock(&pedidos[r]);
    pthread_mutex_unlock(&motos[r]);
    
    return NULL;
}

int main()
{

    setlocale(LC_ALL, "Portuguese");

    pthread_t threads[NUM_ENTREGADORES];
    int ids[NUM_ENTREGADORES];

    // Inicializa os mutexes
    for (int i = 0; i < NUM_RESTAURANTES; i++)
    {
        pthread_mutex_init(&motos[i], NULL);
        pthread_mutex_init(&pedidos[i], NULL);
    }

    // Cria os entregadores (metade veterano, metade novato)
    for (int i = 0; i < NUM_ENTREGADORES; i++)
    {
        ids[i] = i;
        if (i % 2 == 0)
        {
            pthread_create(&threads[i], NULL, entregador_veterano, &ids[i]);
        }
        else
        {
            pthreads_create(&threads[i], NULL, entregador_novato, &ids[i]);
        }
    }

    // Espera as threads (neste simulador elas podem rodar infinitamente)
    for (int i = 0; i < NUM_ENTREGADORES; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}