/*
 * Nome do programa: Clone com Threads
 * Objetivo do programa: Demonstrar o comportamento da função clone() em um programa multithread.
 * Nome do programador: Luara Perilli 202200841 e Beatriz Nascimento 2023007113
 * Data de criação: 25/09/2024
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>

#define STACK_SIZE 1024*1024  // Tamanho da pilha para o clone

// Função que será executada pelo processo clonado
int clone_function(void* arg) {
    printf("Processo clone: PID = %d, PPID = %d\n", getpid(), getppid());
    return 0;
}

// Função executada pela thread criada
void* thread_function(void* arg) {
    char* stack = malloc(STACK_SIZE);  // Aloca espaço para a pilha
    if (!stack) {
        perror("Erro ao alocar pilha para clone");
        exit(EXIT_FAILURE);
    }

    printf("Thread: executando, criando clone...\n");

    // Processo clone criado usando clone()
    pid_t pid = clone(clone_function, stack + STACK_SIZE, SIGCHLD, NULL);

    // Verifica se houve erro ao criar o clone
    if (pid < 0) {
        perror("Erro no clone");
        free(stack);
        exit(EXIT_FAILURE);
    }

    // Espera o processo clone terminar
    if (waitpid(pid, NULL, 0) == -1) {
        perror("Erro no waitpid");
        free(stack);
        exit(EXIT_FAILURE);
    }

    printf("Thread: clone finalizado.\n");
    free(stack);  // Libera a memória da pilha
    return NULL;
}

int main() {
    pthread_t thread_id;

    // Cria uma thread
    if (pthread_create(&thread_id, NULL, thread_function, NULL)) {
        fprintf(stderr, "Erro ao criar a thread\n");
        return 1;
    }

    // Aguarda a conclusão da thread
    if (pthread_join(thread_id, NULL)) {
        fprintf(stderr, "Erro ao aguardar a thread\n");
        return 2;
    }

    printf("Thread terminou, finalizando o processo principal.\n");
    return 0;
}
