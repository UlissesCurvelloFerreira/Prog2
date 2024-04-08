#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo.csv>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *nomeArquivo = argv[1];
    Funcionario funcionarios[MAX_COL];
    int numFuncionarios = lerArquivoCSV(nomeArquivo, funcionarios, MAX_COL);

    printf("Total de Funcionarios lidos: %d\n", numFuncionarios);

    printf("Deseja exibir a descrição dos dados? (1 - Sim, 2 - Não): ");
    int opcao;
    scanf("%d", &opcao);

    if (opcao == 1) {
        printf("Entre com a variavel: ");
        char variavel[20];
        scanf("%s", variavel);

        // Coletar os valores da variável escolhida
        int valores[MAX_COL];
        for (int i = 0; i < numFuncionarios; i++) {
            if (strcmp(variavel, "Nome") == 0) {
                // Tratamento específico para a variável Nome
                // (não estamos lidando com valores numéricos)
                valores[i] = strlen(funcionarios[i].nome);
            } else if (strcmp(variavel, "Sexo") == 0) {
                // Tratamento específico para a variável Sexo
                // (não estamos lidando com valores numéricos)
                valores[i] = funcionarios[i].sexo;
            } else if (strcmp(variavel, "Idade") == 0) {
                valores[i] = funcionarios[i].idade;
            } else if (strcmp(variavel, "Profissao") == 0) {
                // Tratamento específico para a variável Profissao
                // (não estamos lidando com valores numéricos)
                valores[i] = strlen(funcionarios[i].profissao);
            } else if (strcmp(variavel, "Salario") == 0) {
                valores[i] = (int) funcionarios[i].salario;
            } else if (strcmp(variavel, "EC") == 0) {
                // Tratamento específico para a variável EC
                // (não estamos lidando com valores numéricos)
                valores[i] = strlen(funcionarios[i].estadoCivil);
            } else {
                printf("Variável inválida.\n");
                return EXIT_FAILURE;
            }
        }

        // Exibir estatísticas
        printf("Total de Dados: %d\n", numFuncionarios);
        printf("Media: %.2f\n", calcularMedia(valores, numFuncionarios));
        printf("Mediana: %d\n", calcularMediana(valores, numFuncionarios));
        int moda, quantidade;
        moda = calcularModa(valores, numFuncionarios, &quantidade);
        printf("Moda: %d (%d vezes)\n", moda, quantidade);
        printf("Desvio Padrao: %.2f\n", calcularDesvioPadrao(valores, numFuncionarios));
        printf("Minimo: %d\n", encontrarMinimo(valores, numFuncionarios));
        printf("Maximo: %d\n", encontrarMaximo(valores, numFuncionarios));

        int valoresUnicos[MAX_COL];
        int numValoresUnicos = contarValoresUnicos(valores, numFuncionarios, valoresUnicos);
        printf("Numero de valores unicos: %d\n", numValoresUnicos);

        printf("Pressione ENTER para continuar...\n");
        while (getchar() != '\n'); // Limpa o buffer de entrada
        getchar(); // Aguarda o usuário pressionar ENTER
    }

    printf("Programa encerrado.\n");

    return EXIT_SUCCESS;
}
