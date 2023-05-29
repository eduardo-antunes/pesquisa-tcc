/*GRUPO 3
Eduardo Antunes dos Santos Vieira - 5076
Gabriel Benez Duarte Costa - 4701
Lucas Fonseca Sabino Lana - 5105
Pedro Augusto Martins Pereira - 4692
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wctype.h>
#include <wchar.h>

#include "patricia.h"
#include "leitura.h"

// Lê o arquivo de entrada principal a partir da segunda linha, processando
// cada arquivo na lista nele contida individualmente. Esse processamento
// insere todos os termos identificados na árvore patrícia
void input_archive(FILE *input, char *filenames[], Patricia* pat) {
    char name[64];
    int file_id = 0;
    // Percorre a entrada principal, processando cada arquivo na lista
    while(file_id < pat->nr_files) {
        // Cada linha individual corresponde a um arquivo
        if(fgets(name, 64, input) == NULL) {
            fprintf(stderr, "Arquivo de entrada principal mais curto que o esperado\n");
            break;
        }
        name[strcspn(name, "\r\n")] = 0;
        if(name[0] == 0) continue; // linha vazia
        // Salva a associação entre nomes e IDs de arquivos
        filenames[file_id] = (char*) malloc(sizeof(char) * 64);
        strncpy(filenames[file_id], name, 64);

        // Abre o arquivo de TCC cujo nome foi lido
        FILE *tcc = fopen(name, "r, ccs=UTF-8");
        if (tcc == NULL) {
            // Impossível abrir o arquivo, seu processamento é pulado
            fprintf(stderr, "Não foi possível abrir o arquivo '%s'\n", name);
            continue;
        }
        // Chama a função que processa cada arquivo individualmente
        readout_archive(tcc, file_id, pat);
        // Fecha o arquivo TCC atual
        fclose(tcc);
        ++file_id;
    }
}

// Processa um único arquivo na lista, iterando sobre todas as suas palavras
// e inserindo-as na árvore patrícia
void readout_archive(FILE* tcc, int file_id, Patricia* pat) {
    // Declarações iniciais: buffer de palavra, de linha e de estado
    wchar_t *palavra, line[1024], *ptr;
    // Loop para ler o arquivo linha por linha
    while(fgetws(line, 1024, tcc) != NULL) {

        // Processamento inicial de cada linha: remoção da quebra de linha no
        // final, remoção de sinais de pontuação e conversão para minúsculas
        line[wcscspn(line, L"\r\n")] = 0;
        for(int i = 0; line[i] != 0;i++)
            line[i] = iswpunct(line[i]) ? ' ' : towlower(line[i]);

        // Loop para ler a linha palavra por palavra
        palavra = wcstok(line, L" ", &ptr);
        while(palavra != NULL) {
            // A palavra atual tem sua contagem para o presente arquivo
            // incrementada na árvore patrícia. Note que, se ela não existir
            // na árvore, essa função faz a sua inserção
            patricia_update(pat, palavra, file_id);
            palavra = wcstok(NULL, L" ", &ptr);
        }
    }
}
