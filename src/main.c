#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <math.h>
#include "patricia.h"
#include <wchar.h>
#include <string.h>
#include <wctype.h>
#include <stdlib.h>
#include "leitura.h"
#define max_tam 4
#define max_lenght 1024


// Função para criação de um vetor de strings wchar para o input de termos do usuário
wchar_t **vetor_aloc(int num_string, int tam_strings){
    wchar_t **temp = (wchar_t **) malloc(num_string *  sizeof(wchar_t *));

    for(int i =0; i < num_string; i++){
        temp[i] = malloc(tam_strings * sizeof(wchar_t));
    }

    return temp;
}

int main(void) {
    Patricia pat;
    int nr_files = 5;
    int ni[nr_files];
    patricia_init(&pat, nr_files); 

    input_archive(&pat);
    doc_relevance temp_relevance[nr_files];
    

    

    wchar_t **input_words  = vetor_aloc(max_tam, max_lenght);


    

    int terms_num;
    wprintf(L"termos:");
    
    wscanf(L"%d",&terms_num);
    fflush(stdin);

    for(int i = 0; i < terms_num; i++){
        wscanf(L"%ls[^\r\n]", input_words[i]);
        fflush(stdin);

    }



    patricia_count(&pat, ni);
    for(int i =0; i < nr_files; i++){
        wprintf(L"termos distintos doc %d: %d\n",i,ni[i]);
    }
            
    


    user_relevance(input_words, terms_num, &pat, nr_files, 7, temp_relevance); // falta a qtdade de terms !=

    
    
    for(int i=0; i < nr_files; i++){
        wprintf(L"%.2f\n",temp_relevance[i].relevance);
    }

    patricia_free(&pat);

    return 0;
}
