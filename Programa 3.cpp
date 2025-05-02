#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define QTD_SENSORES 5
#define LEITURAS_POR_SENSOR 1000
#define TAM_ID 20

const char *sensores[QTD_SENSORES] = {"TEMP", "PRES", "VIBR", "UMID", "FLUX"};

time_t gerar_timestamp_aleatorio(int dia, int mes, int ano) {
    struct tm t_inicio = {0}, t_fim = {0};

    t_inicio.tm_year = ano - 1900;
    t_inicio.tm_mon = mes - 1;
    t_inicio.tm_mday = dia;
    t_inicio.tm_hour = 0;
    t_inicio.tm_min = 0;
    t_inicio.tm_sec = 0;

    t_fim = t_inicio;
    t_fim.tm_hour = 23;
    t_fim.tm_min = 59;
    t_fim.tm_sec = 59;

    time_t inicio = mktime(&t_inicio);
    time_t fim = mktime(&t_fim);

    if (inicio == -1 || fim == -1) {
        printf("Erro ao gerar timestamps.\n");
        return -1;
    }

    return inicio + rand() % (fim - inicio + 1);
}

float gerar_valor_aleatorio() {
    return ((rand() % 10000) / 100.0);
}

int main() {
    int dia, mes, ano;
    printf("Digite a data desejada (dd mm aaaa): ");
    scanf("%d %d %d", &dia, &mes, &ano);

    srand(time(NULL));

    FILE *saida = fopen("leituras.txt", "w");
    if (!saida) {
        perror("Erro ao criar arquivo de saída");
        return 1;
    }

    for (int i = 0; i < QTD_SENSORES; i++) {
        for (int j = 0; j < LEITURAS_POR_SENSOR; j++) {
            time_t t = gerar_timestamp_aleatorio(dia, mes, ano);
            float valor = gerar_valor_aleatorio();
            fprintf(saida, "%ld %s %.2f\n", t, sensores[i], valor);
        }
    }

    fclose(saida);
    printf("Arquivo de teste 'leituras.txt' gerado com sucesso.\n");
    return 0;
}
