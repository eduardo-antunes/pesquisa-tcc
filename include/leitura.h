/*GRUPO 3
Eduardo Antunes dos Santos Vieira - 5076
Gabriel Benez Duarte Costa - 4701
Lucas Fonseca Sabino Lana - 5105
Pedro Augusto Martins Pereira - 4692
*/

#include <stdio.h>
#include "patricia.h"

// Lê o arquivo de entrada principal a partir da segunda linha, processando
// cada arquivo na lista nele contida individualmente. Esse processamento
// insere todos os termos identificados na árvore patrícia
void input_archive(FILE *input, char *filenames[], Patricia* pat);

// Processa um único arquivo na lista, iterando sobre todas as suas palavras
// e inserindo-as na árvore patrícia
void readout_archive(FILE* tcc, int file_id, Patricia* pat);
