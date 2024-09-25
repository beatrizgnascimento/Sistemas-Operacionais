/*
 * Nome do programa: Fork com Threads
 * Objetivo do programa: Demonstrar o comportamento da função fork() em um programa multithread.
 * Nome do programador: Luara Perilli 202200841 e Beatriz Nascimento 2023007113
 * Data de criação: 25/09/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Função executada pela thread criada. Demonstra o uso de fork().
void* thread_function(void* arg) {
    // Exibe uma mensagem indicando que a thread está executando
    printf("Thread: executando...\n");

    // Processo pai chama fork
    pid_t pid = fork();

    // Verifica se houve erro no fork
    if (pid < 0) {
        perror("Erro no fork");
        exit(EXIT_FAILURE);
    } 
    // Bloco executado no processo filho
    else if (pid == 0) {  
        printf("Processo filho: PID = %d, Thread continua executando...\n", getpid());
        _exit(0); // Termina o processo filho de maneira segura
    } 
    // Bloco executado no processo pai
    else {  
        printf("Processo pai: PID = %d, filho criado com PID = %d\n", getpid(), pid);
    }

    return NULL;
}

int main() {
    pthread_t thread_id;

    // Cria uma nova thread
    if (pthread_create(&thread_id, NULL, thread_function, NULL)) {
        fprintf(stderr, "Erro ao criar a thread\n");
        return 1;
    }

    // Aguarda a conclusão da thread
    if (pthread_join(thread_id, NULL)) {
        fprintf(stderr, "Erro ao aguardar a thread\n");
        return 2;
    }

    printf("Thread terminou, finalizando o processo.\n");
    return 0;
}
