#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEITURAS 100000
#define MAX_SENSORES 100
#define TAM_ID 20

typedef struct {
    long long timestamp;
    char id_sensor[TAM_ID];
    float valor;
} Leitura;

int comparar_leituras(const void *a, const void *b) {
    const Leitura *l1 = (const Leitura *)a;
    const Leitura *l2 = (const Leitura *)b;

    if (l1->timestamp < l2->timestamp) return -1;
    if (l1->timestamp > l2->timestamp) return 1;
    return 0;
}

int sensor_existe(char sensores[][TAM_ID], int total, const char *id) {
    for (int i = 0; i < total; i++) {
        if (strcmp(sensores[i], id) == 0)
            return 1;
    }
    return 0;
}

void adicionar_sensor(char sensores[][TAM_ID], int *total, const char *id) {
    strcpy(sensores[*total], id);
    (*total)++;
}

int main() {
    char nome_entrada[100];
    printf("Digite o nome do arquivo de entrada: ");
    scanf("%s", nome_entrada);

    FILE *arquivo = fopen(nome_entrada, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    Leitura *leituras = malloc(MAX_LEITURAS * sizeof(Leitura));
    if (!leituras) {
        printf("Erro de memória!\n");
        fclose(arquivo);
        return 1;
    }

    int total_leituras = 0;
    while (fscanf(arquivo, "%lld %s %f",
                  &leituras[total_leituras].timestamp,
                  leituras[total_leituras].id_sensor,
                  &leituras[total_leituras].valor) == 3) {
        total_leituras++;
    }
    fclose(arquivo);

    char sensores[MAX_SENSORES][TAM_ID];
    int total_sensores = 0;

    for (int i = 0; i < total_leituras; i++) {
        if (!sensor_existe(sensores, total_sensores, leituras[i].id_sensor)) {
            adicionar_sensor(sensores, &total_sensores, leituras[i].id_sensor);
        }
    }

    for (int i = 0; i < total_sensores; i++) {
        Leitura *filtro = malloc(total_leituras * sizeof(Leitura));
        if (!filtro) {
            printf("Erro de memória ao filtrar sensor %s\n", sensores[i]);
            free(leituras);
            return 1;
        }

        int count = 0;
        for (int j = 0; j < total_leituras; j++) {
            if (strcmp(leituras[j].id_sensor, sensores[i]) == 0) {
                filtro[count++] = leituras[j];
            }
        }

        qsort(filtro, count, sizeof(Leitura), comparar_leituras);

        char nome_saida[50];
        snprintf(nome_saida, sizeof(nome_saida), "%s.txt", sensores[i]);
        FILE *saida = fopen(nome_saida, "w");
        if (!saida) {
            perror("Erro ao criar arquivo de saída");
            free(filtro);
            free(leituras);
            return 1;
        }

        for (int j = 0; j < count; j++) {
            fprintf(saida, "%lld %s %.2f\n",
                    filtro[j].timestamp,
                    filtro[j].id_sensor,
                    filtro[j].valor);
        }

        fclose(saida);
        free(filtro);
    }

    free(leituras);
    printf("Organização completa!\n");
    return 0;
}
