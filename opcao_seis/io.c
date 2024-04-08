#include <stdio.h>
#include <string.h>
#include "io.h"

int lerArquivoCSV(char *nomeArquivo, Funcionario funcionarios[], int maxFuncionarios) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
        return 0;
    }

    int numFuncionarios = 0;
    char linha[100]; // Tamanho da linha do arquivo
    while (fgets(linha, sizeof(linha), arquivo) != NULL && numFuncionarios < maxFuncionarios) {
        // Implemente o código para processar a linha e preencher a estrutura Funcionario
        // Exemplo: sscanf(linha, "%s %c %d %s %f %s", funcionarios[numFuncionarios].nome, &funcionarios[numFuncionarios].sexo, &funcionarios[numFuncionarios].idade, funcionarios[numFuncionarios].profissao, &funcionarios[numFuncionarios].salario, funcionarios[numFuncionarios].estadoCivil);
        numFuncionarios++;
    }

    fclose(arquivo);
    return numFuncionarios;
}

void imprimirVariaveisSelecionadas(Funcionario funcionarios[], int numFuncionarios, char *variaveisSelecionadas[], int numVariaveis) {
    printf("Cabeçalho com as variáveis selecionadas:\n");
    for (int i = 0; i < numVariaveis; i++) {
        printf("%s ", variaveisSelecionadas[i]);
    }
    printf("\n");

    for (int i = 0; i < numFuncionarios; i++) {
        for (int j = 0; j < numVariaveis; j++) {
            if (strcmp(variaveisSelecionadas[j], "Nome") == 0) {
                printf("%s ", funcionarios[i].nome);
            } else if (strcmp(variaveisSelecionadas[j], "Sexo") == 0) {
                printf("%c ", funcionarios[i].sexo);
            } else if (strcmp(variaveisSelecionadas[j], "Idade") == 0) {
                printf("%d ", funcionarios[i].idade);
            } else if (strcmp(variaveisSelecionadas[j], "Profissao") == 0) {
                printf("%s ", funcionarios[i].profissao);
            } else if (strcmp(variaveisSelecionadas[j], "Salario") == 0) {
                printf("%.2f ", funcionarios[i].salario);
            } else if (strcmp(variaveisSelecionadas[j], "EC") == 0) {
                printf("%s ", funcionarios[i].estadoCivil);
            }
        }
        printf("\n");
    }
}

void gravarVariaveisSelecionadas(char *nomeArquivo, Funcionario funcionarios[], int numFuncionarios, char *variaveisSelecionadas[], int numVariaveis) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s para gravação.\n", nomeArquivo);
        return;
    }

    for (int i = 0; i < numFuncionarios; i++) {
        for (int j = 0; j < numVariaveis; j++) {
            if (strcmp(variaveisSelecionadas[j], "Nome") == 0) {
                fprintf(arquivo, "%s,", funcionarios[i].nome);
            } else if (strcmp(variaveisSelecionadas[j], "Sexo") == 0) {
                fprintf(arquivo, "%c,", funcionarios[i].sexo);
            } else if (strcmp(variaveisSelecionadas[j], "Idade") == 0) {
                fprintf(arquivo, "%d,", funcionarios[i].idade);
            } else if (strcmp(variaveisSelecionadas[j], "Profissao") == 0) {
                fprintf(arquivo, "%s,", funcionarios[i].profissao);
            } else if (strcmp(variaveisSelecionadas[j], "Salario") == 0) {
                fprintf(arquivo, "%.2f,", funcionarios[i].salario);
            } else if (strcmp(variaveisSelecionadas[j], "EC") == 0) {
                fprintf(arquivo, "%s,", funcionarios[i].estadoCivil);
            }
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    printf("Arquivo gravado com sucesso.\n");
}
