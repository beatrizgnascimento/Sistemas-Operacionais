/*
 * Nome do programa: Exec com Threads
 * Objetivo do programa: Demonstrar o comportamento de exec() em uma thread.
 * Nome do programador: Luara Perilli 202200841 e Beatriz Nascimento 2023007113
 * Data de criação: 25/09/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Função que será executada pela thread
void* thread_function(void* arg) {
    printf("Thread: executando...\n");

    // Thread chama exec, substituindo o programa atual
    printf("Thread: chamando exec...\n");
    execl("/bin/ls", "ls", NULL);

    // Caso o exec falhe (nunca retorna se tiver sucesso)
    perror("Erro ao chamar exec");
    return NULL;
}

int main() {
    pthread_t thread_id;

    // Cria uma thread
    if (pthread_create(&thread_id, NULL, thread_function, NULL)) {
        fprintf(stderr, "Erro ao criar a thread\n");
        return 1;
    }

    // Aguarda a thread finalizar (nunca será chamado se exec tiver sucesso)
    if (pthread_join(thread_id, NULL)) {
        fprintf(stderr, "Erro ao aguardar a thread\n");
        return 2;
    }

    printf("Thread terminou, finalizando o processo principal.\n");
    return 0;
}
