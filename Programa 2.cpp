#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100000
#define TAM_ID 20

typedef struct {
    long long timestamp;
    char id_sensor[TAM_ID];
    float valor;
} Leitura;

time_t capturar_timestamp_usuario() {
    int dia, mes, ano, hora, minuto, segundo;
    struct tm t;

    printf("Digite a data e hora (dd mm aaaa hh mm ss): ");
    scanf("%d %d %d %d %d %d", &dia, &mes, &ano, &hora, &minuto, &segundo);

    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = minuto;
    t.tm_sec = segundo;
    t.tm_isdst = -1;

    return mktime(&t);
}

int buscar_proximo(Leitura *vetor, int n, long long alvo) {
    int esquerda = 0, direita = n - 1;
    int melhor = 0;
    long long diff_min = llabs(vetor[0].timestamp - alvo);

    while (esquerda <= direita) {
        int meio = (esquerda + direita) / 2;
        long long diff = llabs(vetor[meio].timestamp - alvo);

        if (diff < diff_min) {
            melhor = meio;
            diff_min = diff;
        }

        if (vetor[meio].timestamp < alvo)
            esquerda = meio + 1;
        else
            direita = meio - 1;
    }

    return melhor;
}

int main() {
    char nome_sensor[TAM_ID];
    printf("Digite o nome do sensor (ex: TEMP): ");
    scanf("%s", nome_sensor);

    char nome_arquivo[50];
    snprintf(nome_arquivo, sizeof(nome_arquivo), "%s.txt", nome_sensor);

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo do sensor");
        return 1;
    }

    Leitura *leituras = malloc(MAX * sizeof(Leitura));
    if (!leituras) {
        printf("Erro de memória!\n");
        fclose(arquivo);
        return 1;
    }

    int total = 0;
    while (fscanf(arquivo, "%lld %s %f", &leituras[total].timestamp, leituras[total].id_sensor, &leituras[total].valor) == 3) {
        total++;
    }
    fclose(arquivo);

    time_t alvo = capturar_timestamp_usuario();
    int indice = buscar_proximo(leituras, total, alvo);

    printf("\nLeitura mais próxima:\n");
    printf("Timestamp: %lld\n", leituras[indice].timestamp);
    printf("Sensor: %s\n", leituras[indice].id_sensor);
    printf("Valor: %.2f\n", leituras[indice].valor);

    free(leituras);
    return 0;
}
