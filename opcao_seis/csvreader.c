#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

#define MAX_FUNCIONARIOS 1000
#define MAX_VARIAVEIS 10
#define MAX_NOME_ARQUIVO 100

int main() {
    Funcionario funcionarios[MAX_FUNCIONARIOS];
    char *variaveisSelecionadas[MAX_VARIAVEIS]; // Alterado para ponteiro para string
    int numFuncionarios, numVariaveis;
    char nomeArquivo[MAX_NOME_ARQUIVO];
    char input[100];

    // Ler arquivo CSV
    numFuncionarios = lerArquivoCSV("funcionarios.csv", funcionarios, MAX_FUNCIONARIOS);
    if (numFuncionarios == 0) {
        printf("Erro ao ler o arquivo CSV.\n");
        return 1;
    }

    // Menu principal
    while (1) {
        printf("\n1. Selecionar variáveis\n");
        printf("2. Finalizar\n");
        printf("Escolha uma opção: ");
        fgets(input, sizeof(input), stdin);
        if (strcmp(input, "1\n") == 0) {
            printf("Entre com as variáveis que deseja selecionar (separadas por espaço): ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0; // Remover o caractere de nova linha
            char *token = strtok(input, " ");
            numVariaveis = 0;
            while (token != NULL && numVariaveis < MAX_VARIAVEIS) {
                variaveisSelecionadas[numVariaveis] = malloc(strlen(token) + 1); // Aloca memória para a string
                strcpy(variaveisSelecionadas[numVariaveis], token);
                numVariaveis++;
                token = strtok(NULL, " ");
            }
            imprimirVariaveisSelecionadas(funcionarios, numFuncionarios, variaveisSelecionadas, numVariaveis);
            printf("\nDeseja gravar um arquivo com as variáveis selecionadas? [S|N]: ");
            fgets(input, sizeof(input), stdin);
            if (input[0] == 'S' || input[0] == 's') {
                printf("Entre com o nome do arquivo: ");
                fgets(nomeArquivo, sizeof(nomeArquivo), stdin);
                nomeArquivo[strcspn(nomeArquivo, "\n")] = 0; // Remover o caractere de nova linha
                gravarVariaveisSelecionadas(nomeArquivo, funcionarios, numFuncionarios, variaveisSelecionadas, numVariaveis);
            }
            // Liberar memória alocada para as strings
            for (int i = 0; i < numVariaveis; i++) {
                free(variaveisSelecionadas[i]);
            }
        } else if (strcmp(input, "2\n") == 0) {
            break;
        } else {
            printf("Opção inválida.\n");
        }
    }

    return 0;
}
