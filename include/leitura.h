#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wctype.h>

#include "patricia.h"

//Lê o arquivo "entrada.txt" e analisa quantos arquivos de TCC
//têm. Lê as linhas e abre como arquivo cada linha como arquivo.
int* input_archive (Patricia* pat);

//Recebe uma linha do arquivo "entrada.txt" como endereço de um
//arquivo, Lê o arquivo e insere cada palavra do arquivo em uma
//arvore patricia.
void readout_archive (FILE* TCC, int file_id,Patricia* pat,int* counts);