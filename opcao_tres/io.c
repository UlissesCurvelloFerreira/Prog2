#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

int lerArquivoCSV(char *nomeArquivo, Funcionario funcionarios[], int maxFuncionarios) {
    FILE *arquivo;
    int contador = 0;
    char linha[100];

    arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(EXIT_FAILURE);
    }

    // Ignora a primeira linha (cabeçalho)
    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo) != NULL && contador < maxFuncionarios) {
        sscanf(linha, "%[^,], %c, %d, %[^,], %f, %[^\n]", funcionarios[contador].nome,
               &funcionarios[contador].sexo, &funcionarios[contador].idade,
               funcionarios[contador].profissao, &funcionarios[contador].salario,
               funcionarios[contador].estadoCivil);
        contador++;
    }

    fclose(arquivo);
    return contador;
}

void imprimirFuncionario(Funcionario f) {
    printf("%s %c %d %s %.2f %s\n", f.nome, f.sexo, f.idade, f.profissao, f.salario, f.estadoCivil);
}

void gravarArquivoCSV(char *nomeArquivo, Funcionario funcionarios[], int numFuncionarios) {
    FILE *arquivo;
    arquivo = fopen(nomeArquivo, "w");

    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(arquivo, "ID Nome Sexo Idade Profissao Salario EC\n");

    for (int i = 0; i < numFuncionarios; i++) {
        fprintf(arquivo, "%d %s %c %d %s %.2f %s\n", i, funcionarios[i].nome, funcionarios[i].sexo,
                funcionarios[i].idade, funcionarios[i].profissao, funcionarios[i].salario, funcionarios[i].estadoCivil);
    }

    fclose(arquivo);
}

void filtro(Funcionario funcionarios[], int numFuncionarios, char *variavel, char *filtro, float valor) {
    printf("Resultados:\n");
    printf("ID Nome Sexo Idade Profissao Salario EC\n");

    for (int i = 0; i < numFuncionarios; i++) {
        if (strcmp(variavel, "Idade") == 0) {
            if (strcmp(filtro, "==") == 0) {
                if (funcionarios[i].idade == valor) {
                    imprimirFuncionario(funcionarios[i]);
                }
            } else if (strcmp(filtro, ">") == 0) {
                if (funcionarios[i].idade > valor) {
                    imprimirFuncionario(funcionarios[i]);
                }
            } else if (strcmp(filtro, ">=") == 0) {
                if (funcionarios[i].idade >= valor) {
                    imprimirFuncionario(funcionarios[i]);
                }
            } else if (strcmp(filtro, "<") == 0) {
                if (funcionarios[i].idade < valor) {
                    imprimirFuncionario(funcionarios[i]);
                }
            } else if (strcmp(filtro, "<=") == 0) {
                if (funcionarios[i].idade <= valor) {
                    imprimirFuncionario(funcionarios[i]);
                }
            } else if (strcmp(filtro, "!=") == 0) {
                if (funcionarios[i].idade != valor) {
                    imprimirFuncionario(funcionarios[i]);
                }
            }
        } // implemente condições de filtro para as outras variáveis
    }
}
