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
#include <wctype.h>

#include "patricia.h"
#include "leitura.h"

// Função auxiliar para a alocação de um vetor de strings wchar_t
wchar_t **walloc(int n, int word_size);

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

    // Interface baseada em terminal (GTK de pobre)
    int op, quit = 0;
    do {
        // Listagem textual de operações
        printf("== pesquisa-tcc ==\n");
        printf("1. Ver lista de arquivos considerados\n");
        printf("2. Imprimir o índice invertido\n");
        printf("3. Pesquisar um conjunto de palavras-chave\n");
        printf("4. Sair\n");
        // Escolha da operação a ser realizada
        printf("Escolha uma operação (1-4): ");
        wscanf(L"%d", &op);
        fflush(stdin);

        // Execução da operação escolhida
        int inner_quit = 0;
        wchar_t line[1024];
        switch(op) {
            // Ver lista de arquivos considerados
            case 1:
                printf("\n== Arquivos considerados ==\n");
                for(int file_id = 0; file_id < nr_files; ++file_id)
                    printf("Arquivo #%d: %s\n", file_id, filenames[file_id]);
                printf("\n");
                break;
            // Imprimir o índice invertido
            case 2:
                patricia_print(&pat, stdout);
                break;
            // Pesquisar um conjunto de palavras-chave
            case 3: {
                printf("Palavras-chave de pesquisa: ");
                while(!inner_quit) {
                    if(fgetws(line, 1024, stdin) == NULL) {
                        printf("Por favor, digite novamente\n\n");
                        continue;
                    }
                    line[wcscspn(line, L"\r\n")] = 0;
                    if(line[0] != 0) inner_quit = 1;
                }
                inner_quit = 0;
                // Tratamento inicial dos termos de pesquisa
                int count = 1, n = 0;
                for(int i = 0; line[i] != 0; ++i) {
                    // Contagem de palavras
                    if(iswspace(line[i]))
                        ++count;
                    // Remoção de sinais de pontuação e conversão para minúsculas
                    line[i] = iswpunct(line[i]) ? ' ' : towlower(line[i]);
                }
                // Alocação do vetor de palavras-chave
                wchar_t **words = walloc(count, WORD_SIZE), *word, *state;
                word = wcstok(line, L" ", &state);
                while(word != NULL) {
                    wcsncpy(words[n], word, 64);
                    word = wcstok(NULL, L" ", &state);
                    ++n;
                }
                doc_relevance docs[nr_files];
                user_relevance(words, count, &pat, docs);
                // Exibição do top 5 de arquivos mais relevantes
                int t = 0;
                printf("\n== Top 5 arquivos mais relevantes ==\n");
                for(int i = 0; i < nr_files && t < 5; ++i, ++t) {
                    printf("%d. Arquivo #%d: %s (%g)\n", t + 1, docs[i].file_id,
                        filenames[docs[i].file_id], docs[i].relevance);
                }
                printf("\n");
                // Desalocação do vetor de palavras-chave
                for(int i = 0; i < count; ++i)
                    free(words[i]);
                free(words);
                break;
            }
            // Sair
            case 4:
                quit = 1;
                printf("Obrigado por utilizar pesquisa-tcc\n");
                break;
            default:
                printf("Opção não reconhecida!\n");
                op = 0;
        }
    } while(!quit);

    // Desaloca a memória alocada pelo programa
    patricia_free(&pat);
    for(int file_id = 0; file_id < nr_files; ++file_id)
        free(filenames[file_id]);
    return 0;
}

// Função auxiliar para a alocação de um vetor de strings wchar_t
wchar_t **walloc(int n, int word_size) {
    wchar_t **w = malloc(n * sizeof(wchar_t*));
    for(int i = 0; i < n; ++i)
        w[i] = malloc(word_size * sizeof(wchar_t));
    return w;
}
