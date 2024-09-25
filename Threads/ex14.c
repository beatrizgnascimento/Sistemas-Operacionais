/*
 * Nome do programa: Média e Desvio Padrão de Vetor
 * Objetivo do programa: Criar duas threads que calculam a média e o desvio padrão de um vetor de 20 inteiros.
 * Nome do programador: Luara Perilli 202200841 e Beatriz Nascimento 2023007113
 * Data de criação: 25/09/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#define TAMANHO_VETOR 20

// Estrutura que contém o vetor de números
typedef struct {
    int vetor[TAMANHO_VETOR];
    double media;  // Para armazenar a média
    double desvio_padrao;  // Para armazenar o desvio padrão
} Vetor;

// Função que a thread irá executar para calcular a média dos elementos do vetor
void* calcular_media(void* arg) {
    Vetor* vetor = (Vetor*)arg;
    double soma = 0;

    // Calcula a soma dos elementos
    for (int i = 0; i < TAMANHO_VETOR; i++) {
        soma += vetor->vetor[i];
    }

    vetor->media = soma / TAMANHO_VETOR;
    printf("Média dos elementos do vetor: %.2f\n", vetor->media);

    return NULL;
}

// Função que a thread irá executar para calcular o desvio padrão dos elementos do vetor
void* calcular_desvio_padrao(void* arg) {
    Vetor* vetor = (Vetor*)arg;
    double soma_diferencas = 0;

    // Calcula a soma das diferenças ao quadrado
    for (int i = 0; i < TAMANHO_VETOR; i++) {
        soma_diferencas += pow(vetor->vetor[i] - vetor->media, 2);
    }

    vetor->desvio_padrao = sqrt(soma_diferencas / TAMANHO_VETOR);
    printf("Desvio padrão dos elementos do vetor: %.2f\n", vetor->desvio_padrao);

    return NULL;
}

int main() {
    pthread_t thread_media, thread_desvio;
    Vetor vetor;
    srand(time(NULL));  // Semente para números aleatórios

    // Gera 20 números inteiros aleatórios entre 0 e 100
    printf("Vetor gerado: ");
    for (int i = 0; i < TAMANHO_VETOR; i++) {
        vetor.vetor[i] = rand() % 101;
        printf("%d ", vetor.vetor[i]);
    }
    printf("\n");

    // Cria a thread para calcular a média dos elementos do vetor
    if (pthread_create(&thread_media, NULL, calcular_media, (void*)&vetor)) {
        fprintf(stderr, "Erro ao criar a thread para cálculo da média.\n");
        return 1;
    }

    // Cria a thread para calcular o desvio padrão dos elementos do vetor
    if (pthread_create(&thread_desvio, NULL, calcular_desvio_padrao, (void*)&vetor)) {
        fprintf(stderr, "Erro ao criar a thread para cálculo do desvio padrão.\n");
        return 1;
    }

    // Aguarda a thread da média finalizar
    if (pthread_join(thread_media, NULL)) {
        fprintf(stderr, "Erro ao aguardar a thread da média.\n");
        return 2;
    }

    // Aguarda a thread do desvio padrão finalizar
    if (pthread_join(thread_desvio, NULL)) {
        fprintf(stderr, "Erro ao aguardar a thread do desvio padrão.\n");
        return 2;
    }

    return 0;
}
