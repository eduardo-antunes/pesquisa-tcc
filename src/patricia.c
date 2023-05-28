/*GRUPO 3
Eduardo Antunes dos Santos Vieira - 5076
Gabriel Benez Duarte Costa - 4701
Lucas Fonseca Sabino Lana - 5105
Pedro Augusto Martins Pereira - 4692
*/

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <math.h>

#include "patricia.h"

// Incrementa a contagem para um arquivo específico na lista de pares. Caso
// não haja um par com o id de arquivo procurado, cria-o
static void leaf_inc(Patricia_node *node, int file_id) {
    if(node->node_t != NODE_LEAF) return;
    ++node->as.leaf.counts[file_id];
}

// Aloca um novo nó interno para a árvore patrícia
static Patricia_node *node_alloc(int index, wchar_t ch, Patricia_node *left, Patricia_node *right) {
    Patricia_node *node = (Patricia_node*) malloc(sizeof(Patricia_node));
    node->as.internal.left = left;
    node->as.internal.right = right;
    node->as.internal.index = index;
    node->as.internal.ch = ch;
    node->node_t = NODE_INT;
    return node;
}

// Aloca um novo nó folha para a árvore patrícia
static Patricia_node *leaf_alloc(int nr_files, const wchar_t *word) {
    Patricia_node *node = (Patricia_node*) malloc(sizeof(Patricia_node));
    node->as.leaf.counts = (int*) calloc(nr_files, sizeof(int));
    wcsncpy(node->as.leaf.word, word, WORD_SIZE);
    node->as.leaf.word[WORD_SIZE - 1] = 0;
    node->node_t = NODE_LEAF;
    return node;
}

// Desaloca um nó da árvore patrícia, independente do tipo
static void node_free(Patricia_node *node) {
    if(node->node_t == NODE_LEAF)
        free(node->as.leaf.counts);
    free(node);
}

// Inicializa uma árvore patrícia vazia
void patricia_init(Patricia *pat, int nr_files) {
    pat->root = NULL;
    pat->nr_files = nr_files;
}

// Localiza o primeiro caractere que difere entre duas strings
static int find_diff(const wchar_t *w1, const wchar_t *w2) {
    int i = 0;
    while(w1[i] != 0 && w2[i] != 0) {
        if(w1[i] != w2[i]) return i;
        ++i;
    }
    if(w1[i] != w2[i]) return i; // strings de tamanhos diferentes
    return -1; // strings iguais
}

// Adiciona uma nova palavra na árvore patrícia quando ela não está presente
static void patricia_insert(Patricia_node **root_ptr, int nr_files, int index,
        const wchar_t *word, wchar_t letter, int file_id) {
    // Caso base: ponto de inserção encontrado
    if((*root_ptr)->node_t == NODE_LEAF || index < (*root_ptr)->as.internal.index) {
        Patricia_node *node = leaf_alloc(nr_files, word), *old = *root_ptr;
        if(word[index] <= letter) {
            *root_ptr = node_alloc(index, word[index], node, old);
        } else {
            *root_ptr = node_alloc(index, letter, old, node);
        }
        leaf_inc(node, file_id);
        return;
    }
    // Percurso recursivo pela árvore
    Patricia_node **ptr = word[(*root_ptr)->as.internal.index] <= (*root_ptr)->as.internal.ch ?
        &(*root_ptr)->as.internal.left : &(*root_ptr)->as.internal.right;
    patricia_insert(ptr, nr_files, index, word, letter, file_id);
}

// Incrementa a contagem de uma palavra em particular na patrícia para um
// arquivo em particular. Caso essa palavra ainda não tenha sido registrada
// nesse arquivo ou não exista na patrícia, cria o que for necessário
void patricia_update(Patricia *pat, const wchar_t *word, int file_id) {
    // Caso base: árvore vazia
    if(pat->root == NULL) {
        pat->root = leaf_alloc(pat->nr_files, word);
        leaf_inc(pat->root, file_id);
        return;
    }
    // Percurso iterativo pela árvore
    Patricia_node *node = pat->root;
    while(node->node_t != NODE_LEAF) {
        if(word[node->as.internal.index] <= node->as.internal.ch)
            node = node->as.internal.left;
        else
            node = node->as.internal.right;
    }
    // Localização do primeiro caractere que difere
    int index = find_diff(word, node->as.leaf.word);
    // Se a palavra já estiver na árvore, podemos simplesmente atualizá-la
    if(index < 0) leaf_inc(node, file_id);
    // Do contrário, temos que adicioná-la
    else patricia_insert(&pat->root, pat->nr_files, index, word,
            node->as.leaf.word[index], file_id);
}

// Obtém a lista de contagens para cada arquivo de uma palavra em particular
int *patricia_get(const Patricia *pat, const wchar_t *word) {
    // Caso base: árvore vazia
    if(pat->root == NULL) 
        return NULL;
    // Percurso iterativo pela árvore patrícia
    Patricia_node *node = pat->root;
    while(node->node_t != NODE_LEAF) {
        if(word[node->as.internal.index] <= node->as.internal.ch)
            node = node->as.internal.left;
        else
            node = node->as.internal.right;
    }
    // A palavra que procuramos está na árvore? Se sim, retornamos a sua
    // lista de contagens para cada arquivo
    if(wcscmp(node->as.leaf.word, word) == 0)
        return node->as.leaf.counts;
    return NULL; // de contrário, não há o que fazer
}

// Função auxiliar para a contagem de termos distintos em cada arquivo
static void pcount(Patricia_node *node, int nr_files, int total_count[]) {
    // Caso base: subárvore vazia
    if(node == NULL) return;
    // Caso particular: nó folha, coleção das suas contagens
    if(node->node_t == NODE_LEAF) {
        for(int file_id = 0; file_id < nr_files; ++file_id) {
            // Contamos apenas a primeira ocorrência de cada termo
            if(node->as.leaf.counts[file_id] > 0)
                ++total_count[file_id];
        }
        return;
    }
    // Caso partiular: nó interno, contagem das suas subárvores
    pcount(node->as.internal.left, nr_files, total_count);
    pcount(node->as.internal.right, nr_files, total_count);
}

// Gera um vetor com a contagem de termos distintos em cada arquivo
void patricia_count(const Patricia *pat, int total_count[]) {
    for(int i = 0; i < pat->nr_files; ++i)
        total_count[i] = 0;
    pcount(pat->root, pat->nr_files, total_count);
}

// Função auxiliar para a desalocação
static void pfree(Patricia_node *root) {
    if(root == NULL) return;
    if(root->node_t == NODE_INT) {
        pfree(root->as.internal.left);
        pfree(root->as.internal.right);
    }
    node_free(root);
}

// Desaloca a árvore patrícia
void patricia_free(Patricia *pat) {
    pat->nr_files = 0;
    pfree(pat->root);
}

// Função de auxilio para calcular a variável peso
static float calc_weight(int oc, int dj , int doc_number) {
    //wprintf(L"oc = %d",oc);
    //wprintf(L" dj = %d",dj);
    //wprintf(L" doc_num = %d\n",doc_number);
    float w = oc * (log2f(doc_number) / dj);
    return w;
}

// Função para o calculo do valor relevância baseado em termos para cada documento na coleção
static void TF_IDF(const wchar_t **m, int terms_inputs,  Patricia *pat, int n_docs, doc_relevance *docs){
    int ni[n_docs];
    patricia_count(pat, ni);

    int *counts;
    for(int k = 0; k < n_docs; k++) { 
        float w = 0;
        for(int i =0; i < terms_inputs; i++){
            counts = patricia_get(pat, m[i]);
            if(counts != NULL) {
                for(int file_id = 0; file_id < n_docs; file_id++) {
                    if(file_id == k){
                        int temp = olpat_pair(counts, n_docs);
                        w += calc_weight(counts[file_id], temp, n_docs);

                    }
                }
            }
        }

        float ri = (1/(float)ni[k]) * w;
        docs[k].relevance = ri;
    }
}

// Função para ordenar os documentos por ordem de relevância
static void docs_sort(doc_relevance *docs, int doc_number) {
    int i, j, max;
    doc_relevance aux;
    for(i = 0; i < doc_number - 1; i++){
        max = i;
        for(j = i+ 1; j < doc_number ; j++){
            if(docs[j].relevance > docs[max].relevance){
                max = j;
            }
        }
        aux = docs[max];
        docs[max] = docs[i];
        docs[i] = aux;
        
        
    }
}
// função que inicializa um array que é usado para armazer os valores de relevância
static void array_init(doc_relevance *docs, int doc_number){
    for(int i =0; i < doc_number; i++){
        docs[i].file_id = i;
    }
}
// Função principal para o calculo e impressão baseado na relevância do documento
void user_relevance(const wchar_t **m, int terms_inputs,  Patricia *pat, int doc_number,  doc_relevance *docs) {
    array_init(docs, doc_number);
    TF_IDF(m, terms_inputs, pat, doc_number, docs);
    docs_sort(docs, doc_number);
}

// função que retorna a quantidade de documentos que possue o termo em seus pares
int olpat_pair(int *array, int n_docs){
    int count = 0;
    for(int i =0; i < n_docs; i++){
        if(array[i] != 0){
            count++;
        }
    }
    return count;
}

