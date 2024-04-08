#ifndef IO_H
#define IO_H

typedef struct {
    char nome[100];
    char sexo;
    int idade;
    char profissao[100];
    float salario;
    char estadoCivil[20];
} Funcionario;

int lerArquivoCSV(char *nomeArquivo, Funcionario funcionarios[], int maxFuncionarios);

void imprimirVariaveisSelecionadas(Funcionario funcionarios[], int numFuncionarios, char *variaveisSelecionadas[], int numVariaveis);

void gravarVariaveisSelecionadas(char *nomeArquivo, Funcionario funcionarios[], int numFuncionarios, char *variaveisSelecionadas[], int numVariaveis);

#endif
