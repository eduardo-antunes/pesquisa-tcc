
#include <wchar.h>
//Lê o arquivo "entrada.txt" e analisa quantos arquivos de TCC
//têm. Lê as linhas e abre como arquivo cada linha como arquivo.
void input_archive (Patricia* pat);

//Recebe uma linha do arquivo "entrada.txt" como endereço de um
//arquivo, Lê o arquivo e insere cada palavra do arquivo em uma
//arvore patricia.
void readout_archive (FILE* TCC, int file_id,Patricia* pat);