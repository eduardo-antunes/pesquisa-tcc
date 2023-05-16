#include <wchar.h>
#include <stdlib.h>
#include "patricia.h"

// Incrementa a contagem para um arquivo específico na lista de pares. Caso
// não haja um par com o id de arquivo procurado, cria-o
static void leaf_inc(Patricia_node *node, int file_id) {
    if(node->node_t != NODE_LEAF) return;
    for(int i = 0; i < node->as.leaf.top; ++i) {
        if(node->as.leaf.pairs[i].file_id == file_id) {
            ++node->as.leaf.pairs[i].nr;
            return;
        }
    }
    // Caso particular: a palavra não foi registrada para esse arquivo
    node->as.leaf.pairs[node->as.leaf.top].file_id = file_id;
    node->as.leaf.pairs[node->as.leaf.top].nr = 1;
    ++node->as.leaf.top;
}

// Aloca um novo nó interno para a árvore patrícia
static Patricia_node *node_alloc(int index, wchar_t ch) {
    Patricia_node *node = (Patricia_node*) malloc(sizeof(Patricia_node));
    node->as.internal.left = node->as.internal.right = NULL;
    node->as.internal.index = index;
    node->as.internal.ch = ch;
    node->node_t = NODE_INT;
    return node;
}

// Aloca um novo nó folha para a árvore patrícia
static Patricia_node *leaf_alloc(int nr_files, const wchar_t *word) {
    Patricia_node *node = (Patricia_node*) malloc(sizeof(Patricia_node));
    node->as.leaf.pairs = (Pair*) malloc(sizeof(Pair) * nr_files);
    wcsncpy(node->as.leaf.word, word, 64);
    node->as.leaf.word[63] = 0;
    node->as.leaf.top = 0;
    node->node_t = NODE_LEAF;
    return node;
}

// Desaloca um nó da árvore patrícia, independente do tipo
static void node_free(Patricia_node *node) {
    if(node->node_t == NODE_LEAF)
        free(node->as.leaf.pairs);
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
static void patricia_insert(Patricia_node **root_ptr, int nr_files, int index, const wchar_t *word, int file_id) {
    // Caso base: ponto de inserção encontrado
    if((*root_ptr)->node_t == NODE_LEAF || index < (*root_ptr)->as.internal.index) {
        Patricia_node *node = leaf_alloc(nr_files, word), *aux = *root_ptr;
        (*root_ptr) = node_alloc(index, word[index]);
        (*root_ptr)->as.internal.left = node;
        (*root_ptr)->as.internal.right = aux;
        leaf_inc(node, file_id);
        return;
    }
    // Percurso recursivo pela árvore
    Patricia_node **ptr = word[(*root_ptr)->as.internal.index] == (*root_ptr)->as.internal.ch ?
        &(*root_ptr)->as.internal.left : &(*root_ptr)->as.internal.right;
    patricia_insert(ptr, nr_files, index, word, file_id);
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
        if(word[node->as.internal.index] == node->as.internal.ch)
            node = node->as.internal.left;
        else
            node = node->as.internal.right;
    }
    // Localização do primeiro caractere que difere
    int index = find_diff(word, node->as.leaf.word);
    // Se a palavra já estiver na árvore, podemos simplesmente atualizá-la
    if(index < 0) leaf_inc(node, file_id);
    // Do contrário, temos que adicioná-la
    else patricia_insert(&pat->root, pat->nr_files, index, word, file_id);
}

// Obtém a lista de pares associada a uma palavra na árvore patrícia
int patricia_pairs(const Patricia_node *node, const wchar_t *word, Pair **pairs) {
    // Caso base: árvore vazia
    if(node == NULL) {
        *pairs = NULL;
        return -1;
    }
    // Caso base: árvore unitária
    if(node->node_t == NODE_LEAF) {
        if(wcscmp(word, node->as.leaf.word) == 0) {
            *pairs = node->as.leaf.pairs;
            return node->as.leaf.top;
        }
        *pairs = NULL;
        return -1;
    }
    // Percurso recursivo pela árvore
    if(word[node->as.internal.index] == node->as.internal.ch)
        return patricia_pairs(node->as.internal.left, word, pairs);
    else
        return patricia_pairs(node->as.internal.right, word, pairs);
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
