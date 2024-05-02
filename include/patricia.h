/*GRUPO 3
Eduardo Antunes dos Santos Vieira - 5076
Gabriel Benez Duarte Costa - 4701
Lucas Fonseca Sabino Lana - 5105
Pedro Augusto Martins Pereira - 4692
*/

#ifndef PESQUISA_TCC_PATRICIA_H
#define PESQUISA_TCC_PATRICIA_H

#define WORD_SIZE 64

#include <wchar.h>

// Tipo de nó na árvore patrícia: interno ou folha
typedef enum { NODE_INT, NODE_LEAF } Patricia_t;

// Tipo para armazenar o id do arquivo junto com a sua relevância no momento
typedef struct{ int file_id; double relevance; } doc_relevance;

// Definição de um nó da patrícia
typedef struct node {
    Patricia_t node_t;
    union {
        struct {
            int index;
            wchar_t ch;
            struct node *left;
            struct node *right;
        } internal;
        struct {
            int *counts;
            wchar_t word[WORD_SIZE];
        } leaf;
    } as;
} Patricia_node;

// Estrutura representando a raiz da patrícia
typedef struct {
    int nr_files;
    Patricia_node *root;
} Patricia;

// Inicializa uma árvore patrícia vazia
void patricia_init(Patricia *pat, int nr_files);

// Incrementa a contagem de uma palavra em particular na patrícia para um
// arquivo em particular. Caso essa palavra ainda não tenha sido registrada
// nesse arquivo ou não exista na patrícia, cria o que for necessário
void patricia_update(Patricia *pat, const wchar_t *word, int file_id);

// Obtém a lista de contagens para cada arquivo de uma palavra em particular
int *patricia_get(const Patricia *pat, const wchar_t *word);

// Gera um vetor com a contagem de termos distintos em cada arquivo
void patricia_count(const Patricia *pat, int total_count[]);

// Direciona ordenadamente para o arquivo dado os conteúdos da árvore patrícia
void patricia_print(const Patricia *pat, FILE *fp);

// Desaloca a árvore patrícia
void patricia_free(Patricia *pat);

// Calcula a relevância e ordena os documentos
void user_relevance(wchar_t *words[], int terms_inputs,  Patricia *pat,
        doc_relevance *docs);

#endif // PESQUISA_TCC_PATRICIA_H
