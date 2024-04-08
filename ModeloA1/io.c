#include "io.h"

char* separa(const char *string){
	char *pos_virgula;

	if (!string) return 0;

	pos_virgula = strchr(string, ',');
	if (!pos_virgula) return 0;

	*pos_virgula = '\0';
	return pos_virgula + 1;
}

	
void formata(unsigned char **formatacao, const unsigned short *tamanhos, const unsigned long quantidade, const unsigned char *string){
	unsigned long i = 0;
	unsigned short j;
	const char *atual, *proximo;

	atual = string;
	while(1){
		proximo = separa(atual);
		for (j = 0; j < tamanhos[i] - strlen(atual); j++) formatacao[i][j] = ' ';
		strcpy(formatacao[i] + j, atual);
		atual = proximo;
		
		i++;
		if ((i == quantidade) || (!atual)) break;
	}
	for(; i < quantidade; i++) formatacao[i][0] = '\0';
}

int main(void){
	//TESTES DAS FUNÇÕES
	char string[4][1024] = {"Nome,Sexo,Idade","Alberto,M,14","Jose,M,20", "Ana,F,30"};
	unsigned short tamanhos[3] = {7,4,5};
	unsigned long quantidade = 3;
	unsigned char *formatacao[3];

	for(int i = 0; i < 3; i++) formatacao[i] = (char*) malloc (tamanhos[i]+1);
	for(int i = 0; i < 4; i++){
		formata(formatacao, tamanhos, quantidade, string[i]);
		for (int j = 0; j < 3; j++) printf("%s\t  ", formatacao[j]);
		printf("\n");
	}

}
