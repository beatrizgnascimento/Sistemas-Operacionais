// Nome do programa: Exibição de Filme com Fãs
// Objetivo do programa: Sincronizar a exibição de um filme com a chegada de fãs
// Nome do programador: Seu Nome
// Data de criação: 02/10/2024

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_FANS_NECESSARIOS 10 // Número de fãs necessários para iniciar a exibição
#define MAX_FANS 20              // Número máximo de fãs que podem chegar

int nFans = 0;               // Contador de fãs
sem_t mutex;                 // Semáforo para controle de acesso ao contador de fãs
sem_t dem;                   // Semáforo para sincronizar a exibição do filme
sem_t fila;                  // Semáforo para controle da fila de fãs

// Função executada por cada fã
void* fan(void* arg) {
    int fan_id = *((int*)arg); // ID do fã

    while (1) {
        // Fã chega e tenta entrar
        sem_wait(&mutex);       // Adquire o semáforo
        nFans++;                // Incrementa o número de fãs
        printf("Fã %d chegou.\n", fan_id);
        
        // Se atingiu o número necessário, sinaliza o demonstrador
        if (nFans == NUM_FANS_NECESSARIOS) { 
            sem_post(&dem);
        }
        sem_post(&mutex);        // Libera o semáforo
        
        sem_wait(&fila);         // Espera para assistir ao filme
        
        // Assiste ao filme
        printf("Fã %d está assistindo o filme.\n", fan_id);
        sleep(2);               // Simula o tempo de assistir ao filme
        
        // Telefonar
        printf("Fã %d está telefonando para casa.\n", fan_id);
        sleep(1);               // Simula o tempo de telefonar
    }
    return NULL;
}

// Função executada pelo demonstrador
void* demonstrator(void* arg) {
    while (1) {
        // Espera até ter 10 fãs para iniciar a exibição
        sem_wait(&dem);         // Espera até que haja fãs prontos
        
        // Atualiza o número de fãs
        sem_wait(&mutex);       // Adquire o semáforo
        int fans_to_watch = nFans; // Guarda o número de fãs prontos para assistir
        nFans -= NUM_FANS_NECESSARIOS; // Remove N fãs do contador (não mais de 10)
        sem_post(&mutex);       // Libera o semáforo
        
        // Exibe o filme
        printf("Demonstrador está exibindo o filme.\n");
        sleep(2);               // Simula o tempo de exibição do filme
        
        // Após a exibição, permite que os fãs que assistiram telefonem
        for (int i = 0; i < NUM_FANS_NECESSARIOS; i++) {
            sem_post(&fila);     // Permite que cada fã que assistiu telefone
        }
    }
    return NULL;
}

int main() {
    pthread_t fans[MAX_FANS];  // Array de threads para os fãs
    pthread_t demo;            // Thread para o demonstrador
    int fan_ids[MAX_FANS];     // Identificadores dos fãs

    // Inicializa os semáforos
    sem_init(&mutex, 0, 1);
    sem_init(&dem, 0, 0);
    sem_init(&fila, 0, 0);

    // Cria a thread do demonstrador
    pthread_create(&demo, NULL, demonstrator, NULL);

    // Cria as threads dos fãs
    for (int i = 0; i < MAX_FANS; i++) {
        fan_ids[i] = i + 1;    // Atribui um ID ao fã
        pthread_create(&fans[i], NULL, fan, (void*)&fan_ids[i]);
    }

    // Aguarda a conclusão das threads (neste caso, nunca termina)
    pthread_join(demo, NULL);
    for (int i = 0; i < MAX_FANS; i++) {
        pthread_join(fans[i], NULL);
    }

    // Destrói os semáforos (nunca alcançado neste exemplo)
    sem_destroy(&mutex);
    sem_destroy(&dem);
    sem_destroy(&fila);

    return 0; // Retorno bem-sucedido do programa
}
