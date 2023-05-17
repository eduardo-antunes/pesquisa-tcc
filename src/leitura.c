#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wctype.h>

#include "patricia.h"
#include "leitura.h"

int* input_archive(Patricia* pat) {
    
    FILE *Input;
    FILE *Temp;
    int* counts;
    
    //Define qual será o arquivo que comtém os arquivos TCC
    Input = fopen("entrada.txt", "r");
    char line[64];
    
    //Recebe o primeiro caracter do "entrada.txt" e converte para um
    // numero, após subtrair 48 de seu valor em ASCII. Sendo esse
    // caracter a qauntidade de arquivos TCC presentes.
    int repet;
    fscanf(Input,"%d",&repet);

    //Alocando dinamicamente o tamnho do vetor "counts" de acordo com a
    // quantidade de arquivos TCC 
    counts= (int*) calloc (repet,(sizeof(int)));

    //caso ERRO: Caso não exita ou não ache o arquivo "entrada.txt"
    if (Input == NULL) {
        return "Erro no arquivo";
    }

    //Passa por todo o arquivo, a partir do número de TCC's, pulando a
    // primeira linha.
    for (int i = 0; i < repet; i++) {
        
        //Lê a linha atual a transformando em um caminho para acessar
        // determinado TCC.
        if (fgets(line, sizeof(line), Input) != NULL) {
            FILE *Temp = fopen(line, "r");
            if (Temp == NULL) {

                //caso ERRO: Caso não exita ou não ache o arquivo TCC
                printf("Erro ao abrir o arquivo %s\n", line);
                continue;
            }
            //Chama a função para passar as plavras do TCC atual para
            // a arvore Patricia.
            readout_archive(Temp,i,pat,counts);

            //Fecha o arquivo TCC atual
            fclose(Temp);
        } else {
            //caso ERRO: Caso o arquivo "entrada.txt" for menor do que
            // o esperado
            printf("Erro: arquivo de entrada inesperadamente curto\n");
            break;
        }
    }
    //Fecha o arquivo "entrada.txt"
    fclose(Input);

    //Retornando o vetor counts
    return counts;

}


void readout_archive(FILE* TCC, int file_id, Patricia* pat, int* counts) {
    //Definindo um tamanho de linha e o tipo da palavra da qual permita
    // mexer com acentos sem grandes preocupações.
    wchar_t *palavra, line[1024],*ptr;

    //Loop para ler de linha a linha do documento
    while(fgetws(line, 1024, TCC) != NULL) {

        //Processo de "decupar" a linha em palvras, transformar tudo em
        // letras minúsculas e trocar todos os sinais de pontuação em 
        // espaços, assim, só lemos as palavras
        line[wcscspn(line, L"\n")] = '\0';
        for(int i = 0; line[i] != '\0';i++)
            line[i] = iswpunct(line[i]) ? ' ' : towlower(line[i]);
        palavra = wcstrtok(line, L" ",&ptr);
        
        //Incrmenta o número de palavras que foram armazenadas em
        // cada arquivo, sendo o índice correspondente com um
        // documento TCC
        counts[file_id]++;
        do {

            //A palavra atual é inserida na árvore Patricia, e depois
            // passa para a proxíma palavras da linha até acabar a linha
            // e voltar para o primeiro "while" para ir para a próxima
            // linha.
            patricia_update(pat,palavra,file_id);
            
            //Incrmenta o número de palavras que foram armazenadas em
            // cada arquivo, sendo o índice correspondente com um
            // documento TCC
            counts[file_id]++;
            palavra = wcstrtok(NULL, L" ",&ptr);
        } while (palavra != NULL);
    }
}