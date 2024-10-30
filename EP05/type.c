#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLUSTER_SIZE 4096  // Tamanho do cluster em bytes

void print_file_info(const char *file_name, long file_size, long wasted_bytes) {
    printf("Arquivo: %s\n", file_name);
    printf("Tamanho do arquivo: %ld bytes\n", file_size);
    printf("Bytes desperdiçados no último cluster: %ld bytes\n", wasted_bytes);
}

void read_ext4_file(FILE *disk_file, const char *file_name) {
    char buffer[CLUSTER_SIZE];
    long file_size = 0;
    long total_clusters = 0;
    long wasted_bytes = 0;
    long start_offset = -1;

    // Lê o disco em clusters
    while (fread(buffer, sizeof(char), CLUSTER_SIZE, disk_file) == CLUSTER_SIZE) {
        total_clusters++;

        // Verifica se o arquivo está no cluster
        if (strstr(buffer, file_name) != NULL) {
            // O arquivo foi encontrado; começamos a ler o conteúdo
            start_offset = ftell(disk_file) - CLUSTER_SIZE + (strstr(buffer, file_name) - buffer);
            char *content_start = strstr(buffer, file_name) + strlen(file_name) + 1; // Pular o nome do arquivo e o null terminator

            // Imprime o conteúdo do arquivo até o fim do cluster ou até encontrar um null terminator
            while (*content_start != '\0' && (content_start - buffer) < CLUSTER_SIZE) {
                putchar(*content_start);
                file_size++;
                content_start++;
            }

            // Calcular bytes desperdiçados
            wasted_bytes = CLUSTER_SIZE - (file_size % CLUSTER_SIZE);
            break; // Saia do loop após encontrar e imprimir o conteúdo
        }
    }

    if (start_offset == -1) {
        printf("Arquivo %s não encontrado.\n", file_name);
    } else {
        print_file_info(file_name, file_size, wasted_bytes);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <nome do arquivo>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *file_name = argv[1];
    FILE *disk_file = fopen("Disco.dat", "rb");

    if (!disk_file) {
        perror("Erro ao abrir o arquivo Disco.dat");
        return EXIT_FAILURE;
    }

    read_ext4_file(disk_file, file_name);
    fclose(disk_file);

    return EXIT_SUCCESS;
}
