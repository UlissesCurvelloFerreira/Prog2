#ifndef IO_H
#define IO_H

#include <stdbool.h>

#define MAX_COL 6 // número máximo de colunas no arquivo CSV

typedef struct {
    char nome[50];
    char sexo;
    int idade;
    char profissao[50];
    float salario;
    char estadoCivil[15];
} Funcionario;

// Funções de leitura e escrita de arquivos
int lerArquivoCSV(char *nomeArquivo, Funcionario funcionarios[], int maxFuncionarios);
void imprimirFuncionario(Funcionario f);
void gravarArquivoCSV(char *nomeArquivo, Funcionario funcionarios[], int numFuncionarios);

// Funções de estatísticas
float calcularMedia(int valores[], int numValores);
int calcularMediana(int valores[], int numValores);
int calcularModa(int valores[], int numValores, int *quantidade);
float calcularDesvioPadrao(int valores[], int numValores);
int encontrarMinimo(int valores[], int numValores);
int encontrarMaximo(int valores[], int numValores);
int contarValoresUnicos(int valores[], int numValores, int valoresUnicos[]);

#endif
