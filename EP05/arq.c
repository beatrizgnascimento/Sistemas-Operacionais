#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARQUIVOS 8
#define MAX_NOME 8
#define MAX_EXTENSAO 4 // Ajustado para 4

typedef struct {
  unsigned short dia;
  unsigned short mes;
  unsigned short ano;
  unsigned short hora;
  unsigned short min;
  unsigned short seg;
} data;

typedef struct {
  char nome[MAX_NOME];
  char extensao[MAX_EXTENSAO];
  unsigned short sistema;
  unsigned short hidden;
  unsigned short archived;
  data criacao;
  data acesso;
  unsigned long int tamanho;
  unsigned long int cluster;
} arquivo;

arquivo diretorio[MAX_ARQUIVOS];
int num_arquivos = 0;

void listar_arquivos(int detalhe, int sistema, int largura) {
  int arquivos_na_linha = 0;

  for (int i = 0; i < num_arquivos; i++) {
    arquivo arq = diretorio[i];

    // Verifica se o arquivo deve ser exibido
    if (arq.nome[0] == 0xEB || arq.hidden != 0 || 
        (sistema == 0 && arq.sistema != 0) || 
        (sistema == 2 && arq.sistema == 0) || (sistema == 2 && strcmp(arq.extensao, "txt") == 0)) {
      continue;
    }

    char nome_completo[13];
    snprintf(nome_completo, sizeof(nome_completo), "%s.%s", arq.nome,
             arq.extensao);

    if (detalhe) {
      // Imprime detalhes do arquivo
      printf("Nome: %s, Tamanho: %lu, Criacao: %02u/%02u/%04u %02u:%02u:%02u\n",
             nome_completo, arq.tamanho, arq.criacao.dia, arq.criacao.mes,
             arq.criacao.ano, arq.criacao.hora, arq.criacao.min,
             arq.criacao.seg);
    } else {
      // Imprime o nome e extensão conforme a largura especificada
      printf("%s", nome_completo);
      arquivos_na_linha++;

      // Controle de espaçamento e quebra de linha
      if (arquivos_na_linha == largura) {
        printf("\n");
        arquivos_na_linha = 0;
      } else {
        printf("   "); // Espaçamento entre arquivos
      }
    }
  }
  if (!detalhe && arquivos_na_linha != 0) {
    printf("\n");
  }
}

int main(int argc, char *argv[]) {
  // Simulação de arquivos para testes
  strcpy(diretorio[0].nome, "file1");
  strcpy(diretorio[0].extensao, "txt");
  diretorio[0].tamanho = 1024;
  diretorio[0].criacao = (data){1, 1, 2024, 12, 30, 15};
  diretorio[0].hidden = 0;
  diretorio[0].sistema = 0;
  num_arquivos++;

  strcpy(diretorio[1].nome, "file2");
  strcpy(diretorio[1].extensao, "log");
  diretorio[1].tamanho = 2048;
  diretorio[1].criacao = (data){2, 1, 2024, 13, 15, 45};
  diretorio[1].hidden = 0;
  diretorio[1].sistema = 1; // Arquivo de sistema
  num_arquivos++;

  strcpy(diretorio[2].nome, "file3");
  strcpy(diretorio[2].extensao, "doc");
  diretorio[2].tamanho = 512;
  diretorio[2].criacao = (data){3, 1, 2024, 9, 0, 0};
  diretorio[2].hidden = 1; // Arquivo oculto
  diretorio[2].sistema = 0;
  num_arquivos++;

  // Adicione outros arquivos de teste se necessário...

  // Processa argumentos de entrada
  if (argc == 1) {
    listar_arquivos(0, 0, 2); // Sem detalhes, 2 arquivos por linha
  } else if (strcmp(argv[1], "-a") == 0) {
    listar_arquivos(1, 0, 1); // Com detalhes, 1 arquivo por linha
  } else if (strcmp(argv[1], "-s") == 0) {
    listar_arquivos(0, 2,
                    2); // Sem detalhes, apenas arquivos de sistema, 2 por linha
  } else if (strcmp(argv[1], "-w") == 0) {
    listar_arquivos(0, 0, 4); // Sem detalhes, 4 arquivos por linha
  } else {
    printf("Opção inválida para arq\n");
    return 1;
  }

  return 0;
}
