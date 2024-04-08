#ifndef IO_H
#define IO_H

#include "funcionario.h"

int lerArquivoCSV(char *nomeArquivo, Funcionario funcionarios[], int maxFuncionarios);
void gravarArquivoCSV(char *nomeArquivo, Funcionario funcionarios[], int numFuncionarios);

#endif
