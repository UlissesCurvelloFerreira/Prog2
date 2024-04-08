#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "io.h"










int main(int argc, char *argv[])
{
    FILE *csv;
    int op;

    if (argc != 2)
    {
        return 1;
    }
    if (!(csv = fopen(argv[1], "r")))
    {
        printf("Erro ao abrir o arquivo %s\n", argv[1]);
        return 1;
    }

    do
    {

        printf("1) Sumário do Arquivo\n");
        printf("2) Mostrar\n");
        printf("3) Fim\n");

        scanf("%d", &op);
        switch (op)
        {
        case 1:
            sumario(csv);
            break;
        case 2:
            mostrar(csv);
            break;
        default:
            break;
        }
    } while (op != 3);

    fclose(csv);

    return 0;
}