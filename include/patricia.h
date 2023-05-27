





#ifndef PESQUISA_TCC_PATRICIA_H
#define PESQUISA_TCC_PATRICIA_H

#include <wchar.h>
#include <math.h>

// Par arquivo-contagem, utilizado para a definição de arquivo invertido
typedef struct { int file_id, nr; } Pair;

// Tipo de nó na árvore patrícia: interno ou folha
typedef enum { NODE_INT, NODE_LEAF } Patricia_t;

// Tipo para armazenar o id do arquivo junto com a sua relevância no momento
typedef struct{int file_id; float relevance}doc_relevance;

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
            int top;
            Pair *pairs;
            wchar_t word[64];
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

// Obtém a lista de pares associada a uma palavra na árvore patrícia
int patricia_pairs(const Patricia_node *node, const wchar_t *word, Pair **pairs);

// Gera um vetor com a contagem de termos distintos em cada arquivo
void patricia_count(const Patricia *pat, int count[]);

// Desaloca a árvore patrícia
void patricia_free(Patricia *pat);


//calcula a relevância e ordena os documentos 
void user_relevance(const wchar_t **m, int terms_inputs,  Patricia *pat, int doc_number, int ni, doc_relevance *docs);
#endif // PESQUISA_TCC_PATRICIA_H
