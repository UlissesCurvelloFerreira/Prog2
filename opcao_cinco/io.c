#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

int lerArquivoCSV(char *nomeArquivo, Funcionario funcionarios[], int maxFuncionarios) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        return -1;
    }

    char linha[1000]; // Ajuste o tamanho conforme necessário
    int numFuncionarios = 0;
    while (fgets(linha, sizeof(linha), arquivo) != NULL && numFuncionarios < maxFuncionarios) {
        // Aqui você deve implementar o código para extrair os dados de cada linha do arquivo CSV
        // e preencher a estrutura Funcionario
    }

    fclose(arquivo);
    return numFuncionarios;
}

void gravarArquivoCSV(char *nomeArquivo, Funcionario funcionarios[], int numFuncionarios) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s para gravação.\n", nomeArquivo);
        return;
    }

    for (int i = 0; i < numFuncionarios; i++) {
        // Aqui você deve implementar o código para gravar os dados de cada funcionário no arquivo CSV
    }

    fclose(arquivo);
}
