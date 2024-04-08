#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

float calcularMedia(int valores[], int numValores) {
    float soma = 0.0;
    for (int i = 0; i < numValores; i++) {
        soma += valores[i];
    }
    return soma / numValores;
}

int calcularMediana(int valores[], int numValores) {
    int mediana;
    int meio = numValores / 2;

    // Primeiro, ordenamos os valores
    for (int i = 0; i < numValores - 1; i++) {
        for (int j = i + 1; j < numValores; j++) {
            if (valores[i] > valores[j]) {
                int temp = valores[i];
                valores[i] = valores[j];
                valores[j] = temp;
            }
        }
    }

    // Se numValores for ímpar, a mediana é o valor do meio
    if (numValores % 2 != 0) {
        mediana = valores[meio];
    } else { // Se for par, a mediana é a média dos dois valores do meio
        mediana = (valores[meio - 1] + valores[meio]) / 2;
    }

    return mediana;
}

int calcularModa(int valores[], int numValores, int *quantidade) {
    int moda = valores[0];
    int maximoFrequencia = 1;

    for (int i = 0; i < numValores; i++) {
        int contador = 1;
        for (int j = i + 1; j < numValores; j++) {
            if (valores[i] == valores[j])
                contador++;
        }
        if (contador > maximoFrequencia) {
            moda = valores[i];
            maximoFrequencia = contador;
        }
    }

    *quantidade = maximoFrequencia;
    return moda;
}

float calcularDesvioPadrao(int valores[], int numValores) {
    float media = calcularMedia(valores, numValores);
    float somaDesvios = 0.0;
    for (int i = 0; i < numValores; i++) {
        somaDesvios += pow(valores[i] - media, 2);
    }
    return sqrt(somaDesvios / numValores);
}

int encontrarMinimo(int valores[], int numValores) {
    int minimo = valores[0];
    for (int i = 1; i < numValores; i++) {
        if (valores[i] < minimo) {
            minimo = valores[i];
        }
    }
    return minimo;
}

int encontrarMaximo(int valores[], int numValores) {
    int maximo = valores[0];
    for (int i = 1; i < numValores; i++) {
        if (valores[i] > maximo) {
            maximo = valores[i];
        }
    }
    return maximo;
}

int contarValoresUnicos(int valores[], int numValores, int valoresUnicos[]) {
    int contador = 0;
    bool encontrado = false;

    for (int i = 0; i < numValores; i++) {
        encontrado = false;
        for (int j = 0; j < contador; j++) {
            if (valores[i] == valoresUnicos[j]) {
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            valoresUnicos[contador] = valores[i];
            contador++;
        }
    }

    return contador;
}
