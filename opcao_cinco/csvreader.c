#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionario.h"
#include "io.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo.csv>\n", argv[0]);
        return 1;
    }

    // Supondo que você já tenha implementado a leitura do arquivo CSV para carregar os funcionários em um array
    Funcionario funcionarios[100000]; // Ajuste o tamanho conforme necessário
    int numFuncionarios = lerArquivoCSV(argv[1], funcionarios, 100000); // O nome do arquivo é passado como argumento

    if (numFuncionarios == -1) {
        printf("Erro ao ler o arquivo CSV.\n");
        return 1;
    }

    char variavel[20];
    char ordem;
    printf("Entre com a variavel: ");
    scanf("%s", variavel);
    printf("Selecione uma opcao [A]scendente ou [D]escrescente: ");
    scanf(" %c", &ordem);

    ordenarDados(funcionarios, numFuncionarios, variavel, ordem);

    printf("0 Nome Sexo Idade Profissao Salario EC\n");
    for (int i = 0; i < numFuncionarios; i++) {
        printf("%d %s %s %.1f %s %.2f %s\n", i, funcionarios[i].nome, funcionarios[i].sexo, funcionarios[i].idade, funcionarios[i].profissao, funcionarios[i].salario, funcionarios[i].estadoCivil);
    }

    char opcao;
    printf("Deseja gravar um arquivo com os dados ordenados? [S|N] ");
    scanf(" %c", &opcao);
    if (opcao == 'S' || opcao == 's') {
        char nomeArquivo[100];
        printf("Entre com o nome do arquivo: ");
        scanf("%s", nomeArquivo);
        gravarArquivoCSV(nomeArquivo, funcionarios, numFuncionarios);
        printf("Arquivo gravado com sucesso.\n");
    }

    printf("Deseja descartar os dados originais? [S|N]: ");
    scanf(" %c", &opcao);
    if (opcao == 'S' || opcao == 's') {
        // Aqui você pode limpar os dados originais, se necessário
    }

    printf("Pressione ENTER para continuar.\n");
    getchar(); // Limpar o buffer de entrada
    getchar(); // Aguardar pressionar ENTER

    return 0;
}
