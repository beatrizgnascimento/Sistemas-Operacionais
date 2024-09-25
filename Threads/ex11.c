/*
 * Nome do programa: Saudação com Hora e Minuto
 * Objetivo do programa: Ler hora e minuto e repassar para uma thread, que imprime uma saudação adequada.
 * Nome do programador: Luara Perilli 202200841 e Beatriz Nascimento 2023007113
 * Data de criação: 25/09/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Estrutura para armazenar a hora e o minuto
typedef struct {
    int hora;
    int minuto;
} HoraMinuto;

// Função que a thread irá executar para imprimir a saudação
void* imprimir_saudacao(void* arg) {
    HoraMinuto* tempo = (HoraMinuto*)arg;  // Recebe os dados da hora e minuto

    // Verifica a hora e imprime a saudação apropriada
    if (tempo->hora >= 5 && tempo->hora < 12) {
        printf("Bom dia!\n");
    } else if (tempo->hora >= 12 && tempo->hora < 18) {
        printf("Boa tarde!\n");
    } else {
        printf("Boa noite!\n");
    }

    return NULL;
}

int main() {
    pthread_t thread_id;
    HoraMinuto tempo;

    // Leitura da hora e minuto
    printf("Digite a hora (0-23): ");
    scanf("%d", &tempo.hora);

    printf("Digite o minuto (0-59): ");
    scanf("%d", &tempo.minuto);

    // Verifica se a hora e minuto são válidos
    if (tempo.hora < 0 || tempo.hora > 23 || tempo.minuto < 0 || tempo.minuto > 59) {
        fprintf(stderr, "Hora ou minuto inválidos.\n");
        return 1;
    }

    // Cria a thread para imprimir a saudação
    if (pthread_create(&thread_id, NULL, imprimir_saudacao, (void*)&tempo)) {
        fprintf(stderr, "Erro ao criar a thread\n");
        return 1;
    }

    // Espera a thread finalizar
    if (pthread_join(thread_id, NULL)) {
        fprintf(stderr, "Erro ao aguardar a thread\n");
        return 2;
    }

    return 0;
}
