#include <stdio.h>
#include <stdlib.h>

#include "patricia.h"

// Lê o arquivo de entrada principal a partir da segunda linha, processando
// cada arquivo na lista nele contida individualmente. Esse processamento
// insere todos os termos identificados na árvore patrícia
void input_archive(FILE *input, int nr_files, Patricia* pat);

// Processa um único arquivo na lista, iterando sobre todas as suas palavras
// e inserindo-as na árvore patrícia
void readout_archive(FILE* tcc, int file_id, Patricia* pat);
