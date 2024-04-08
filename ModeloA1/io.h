#ifndef __IO__
#define __IO__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct arq_csv {
	FILE *arquivo;
	unsigned long linhas;
	unsigned long colunas;
	unsigned long posicao5;
	unsigned short *tam_colunas;
	char *tipos;
} arq_csv;

char* separa(const char *string);
void formata(unsigned char **formatacao, const unsigned short *tamanhos, const unsigned long quantidade, const unsigned char *string);

arq_csv* abre(char *path);
void fecha(arq_csv *csv);
void verifica(arq_csv *csv);

void sumario(arq_csv *csv);
void exibe(arq_csv *csv);


#endif