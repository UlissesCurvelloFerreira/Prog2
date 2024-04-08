#include <stdio.h>
#include <string.h>
#include "io.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo.csv>\n", argv[0]);
        return 1;
    }

    Funcionario funcionarios[1000]; // ajuste o tamanho conforme necessário
    int numFuncionarios = lerArquivoCSV(argv[1], funcionarios, 1000);

    if (numFuncionarios == 0) {
        printf("Nenhum funcionário encontrado no arquivo.\n");
        return 1;
    }

    // Opções de filtro
    printf("Opções de filtro:\n");
    printf("1 - Filtrar\n");
    printf("2 - Sair\n");

    int opcao;
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    while (opcao != 2) {
        if (opcao == 1) {
            char variavel[20];
            char opFiltro[3]; // Agora é uma string para armazenar operadores de comparação com mais de um caractere
            float valor;

            printf("Entre com a variável: ");
            scanf("%s", variavel);

            printf("Escolha um filtro ( == > >= < <= != ): ");
            scanf("%s", opFiltro);

            printf("Digite um valor: ");
            scanf("%f", &valor);

            Funcionario funcionariosFiltrados[1000]; // ajuste o tamanho conforme necessário
            int numFuncionariosFiltrados = 0;

            for (int i = 0; i < numFuncionarios; i++) {
                if (strcmp(variavel, "Idade") == 0) {
                    if (strcmp(opFiltro, "==") == 0) {
                        if (funcionarios[i].idade == valor) {
                            funcionariosFiltrados[numFuncionariosFiltrados++] = funcionarios[i];
                        }
                    } else if (strcmp(opFiltro, ">") == 0) {
                        if (funcionarios[i].idade > valor) {
                            funcionariosFiltrados[numFuncionariosFiltrados++] = funcionarios[i];
                        }
                    } else if (strcmp(opFiltro, ">=") == 0) {
                        if (funcionarios[i].idade >= valor) {
                            funcionariosFiltrados[numFuncionariosFiltrados++] = funcionarios[i];
                        }
                    } else if (strcmp(opFiltro, "<") == 0) {
                        if (funcionarios[i].idade < valor) {
                            funcionariosFiltrados[numFuncionariosFiltrados++] = funcionarios[i];
                        }
                    } else if (strcmp(opFiltro, "<=") == 0) {
                        if (funcionarios[i].idade <= valor) {
                            funcionariosFiltrados[numFuncionariosFiltrados++] = funcionarios[i];
                        }
                    } else if (strcmp(opFiltro, "!=") == 0) {
                        if (funcionarios[i].idade != valor) {
                            funcionariosFiltrados[numFuncionariosFiltrados++] = funcionarios[i];
                        }
                    }
                } // implemente condições de filtro para as outras variáveis
            }

            printf("Resultados:\n");
            printf("ID Nome Sexo Idade Profissao Salario EC\n");
            for (int i = 0; i < numFuncionariosFiltrados; i++) {
                imprimirFuncionario(funcionariosFiltrados[i]);
            }

            char gravar;
            printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: ");
            scanf(" %c", &gravar);
            if (gravar == 'S' || gravar == 's') {
                char nomeArquivo[50];
                printf("Entre com o nome do arquivo: ");
                scanf("%s", nomeArquivo);
                gravarArquivoCSV(nomeArquivo, funcionariosFiltrados, numFuncionariosFiltrados);
                printf("Arquivo gravado com sucesso.\n");
            }

        } else {
            printf("Opção inválida.\n");
        }

        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
    }

    printf("Programa encerrado.\n");

    return 0;
}
