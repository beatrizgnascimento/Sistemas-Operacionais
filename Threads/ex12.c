/*
 * Nome do programa: Soma de Pares e Produto de Ímpares
 * Objetivo do programa: Criar duas threads para calcular a soma dos pares e o produto dos ímpares em um intervalo [a, b].
 * Nome do programador: Luara Perilli 202200841 e Beatriz Nascimento 2023007113
 * Data de criação: 25/09/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Estrutura para armazenar o intervalo [a, b]
typedef struct {
    int a;
    int b;
} Intervalo;

int soma_pares = 0;   // Variável compartilhada para a soma dos números pares
long long produto_impares = 1;  // Variável compartilhada para o produto dos números ímpares

// Função que a primeira thread irá executar para calcular a soma dos pares
void* calcular_soma_pares(void* arg) {
    Intervalo* intervalo = (Intervalo*)arg;  // Recebe os dados do intervalo

    for (int i = intervalo->a; i <= intervalo->b; i++) {
        if (i % 2 == 0) {
            soma_pares += i;  // Soma os valores pares
        }
    }

    return NULL;
}

// Função que a segunda thread irá executar para calcular o produto dos ímpares
void* calcular_produto_impares(void* arg) {
    Intervalo* intervalo = (Intervalo*)arg;  // Recebe os dados do intervalo

    for (int i = intervalo->a; i <= intervalo->b; i++) {
        if (i % 2 != 0) {
            produto_impares *= i;  // Multiplica os valores ímpares
        }
    }

    return NULL;
}

int main() {
    pthread_t thread_pares, thread_impares;
    Intervalo intervalo;

    // Leitura dos valores a e b
    printf("Digite o valor de a: ");
    scanf("%d", &intervalo.a);

    printf("Digite o valor de b: ");
    scanf("%d", &intervalo.b);

    // Verificação se o intervalo é válido
    if (intervalo.a > intervalo.b) {
        fprintf(stderr, "Erro: O valor de a deve ser menor ou igual ao valor de b.\n");
        return 1;
    }

    // Cria a thread para calcular a soma dos pares
    if (pthread_create(&thread_pares, NULL, calcular_soma_pares, (void*)&intervalo)) {
        fprintf(stderr, "Erro ao criar a thread para soma dos pares.\n");
        return 1;
    }

    // Cria a thread para calcular o produto dos ímpares
    if (pthread_create(&thread_impares, NULL, calcular_produto_impares, (void*)&intervalo)) {
        fprintf(stderr, "Erro ao criar a thread para produto dos ímpares.\n");
        return 1;
    }

    // Aguarda a thread da soma dos pares finalizar
    if (pthread_join(thread_pares, NULL)) {
        fprintf(stderr, "Erro ao aguardar a thread da soma dos pares.\n");
        return 2;
    }

    // Aguarda a thread do produto dos ímpares finalizar
    if (pthread_join(thread_impares, NULL)) {
        fprintf(stderr, "Erro ao aguardar a thread do produto dos ímpares.\n");
        return 2;
    }

    // Imprime os resultados
    printf("Soma dos valores pares no intervalo [%d, %d]: %d\n", intervalo.a, intervalo.b, soma_pares);
    printf("Produto dos valores ímpares no intervalo [%d, %d]: %lld\n", intervalo.a, intervalo.b, produto_impares);

    return 0;
}
