#ifndef IO_H
#define IO_H

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

// Função genérica para filtro
void filtro(Funcionario funcionarios[], int numFuncionarios, char *variavel, char *filtro, float valor);

#endif
