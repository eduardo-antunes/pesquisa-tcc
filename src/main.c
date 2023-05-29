/*GRUPO 3
Eduardo Antunes dos Santos Vieira - 5076
Gabriel Benez Duarte Costa - 4701
Lucas Fonseca Sabino Lana - 5105
Pedro Augusto Martins Pereira - 4692
*/

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

#include "patricia.h"
#include "leitura.h"

#define MAX_TAM 2
#define MAX_LENGHT 64

// Função para criação de um vetor de strings wchar para o input de termos do usuário
wchar_t **vetor_aloc(int num_string, int tam_strings){
    wchar_t **temp = (wchar_t **) malloc(num_string *  sizeof(wchar_t *));
    for(int i =0; i < num_string; i++) {
        temp[i] = malloc(tam_strings * sizeof(wchar_t));
    }
    return temp;
}

int main(void) {
    // Português é bom e eu gosto
    setlocale(LC_ALL, "");

    // Abre o arquivo de entrada principal
    FILE *input = fopen("entrada.txt", "r");
    if(input == NULL) {
        fprintf(stderr, "Não foi possível abrir o arquivo 'entrada.txt'\n");
        return 1;
    }
    // Inicializa a patrícia com o número contido na primeira linha do arquivo
    // de entrada, que corresponde à quantidade de arquivos que serão considerados
    // na pesquisa
    Patricia pat; int nr_files;
    fscanf(input, "%d", &nr_files);
    patricia_init(&pat, nr_files);

    // Preenche a árvore patrícia a partir dos arquivos de entrada
    char *filenames[nr_files];
    input_archive(input, filenames, &pat);
    fclose(input);

    // Exibe o índice invertido na saída padrão
    printf("== Legendas ==\n");
    for(int file_id = 0; file_id < nr_files; ++file_id)
        printf("Arquivo #%d: %s\n", file_id, filenames[file_id]);
    printf("\n");
    patricia_print(&pat, stdout);

    /*
    // Caso de teste da função patricia_count
    int count[nr_files];
    doc_relevance relevance_docs[nr_files];
    patricia_count(&pat, count);
    printf("== Informações gerais ==\n");
    for(int i = 0; i < nr_files; ++i)
        printf("O arquivo #%d contém %d termos distintos\n", i, count[i]);
    
    // Teste do cálculo de relevância
    wchar_t **input_words = vetor_aloc(MAX_TAM, MAX_LENGHT);
    for(int i =0; i < MAX_TAM; i++){
        wscanf(L"%ls[^\r\n]",input_words[i]);
    }
    user_relevance(input_words, MAX_TAM, &pat, relevance_docs);

    // Exibição do resultado final
    for(int i =0; i < nr_files; i++){
        printf("Doc %d relevance %.2f\n", relevance_docs[i].file_id, relevance_docs[i].relevance);
    }
    */
    // Desaloca a memória alocada pelo programa
    patricia_free(&pat);
    for(int file_id = 0; file_id < nr_files; ++file_id)
        free(filenames[file_id]);
    return 0;
}
