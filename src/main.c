#include <stdio.h>
#include <math.h>
#include "patricia.h"
#include <wchar.h>
#include <string.h>

#define max_tam 4
#define max_lenght 1024



/*float TF_IDF(const wchar_t **m, int terms_inputs,  Patricia *pat, int N, int doc_id, int ni){
    Pair *pairs;
    int presence;
    float w = 0;
    for(int i =0; i < terms_inputs; i++){
        presence = patricia_pairs(pat->root, m[i], &pairs);
        for(int j = 0; j < presence; j++){
            if(pairs[j].file_id == doc_id + 1){
                w += wcalc_weight(pairs[j].nr, presence, N);
            }
        }
        
    }
    float ri = (1/(float)ni) * w;
    return ri;
}*/



wchar_t **vetor_aloc(int num_string, int tam_strings){
    wchar_t **temp = (wchar_t *) malloc(num_string *  sizeof(wchar_t *));

    for(int i =0; i < num_string; i++){
        temp[i] = malloc(tam_strings * sizeof(wchar_t));
    }

    return temp;
}

int main(void) {
    Patricia pat;
    
    int temp_exp[20] = {7,8};
    int temp_doc_number = 2;
    patricia_init(&pat, 5);

    wchar_t **input_words  = vetor_aloc(max_tam, max_lenght); ;

    Pair *pairs;


    int command = -5;
    while(1){
        printf("digite:\n[1] ---- Para receber o arquivo de entrada com os textos a serem indexados; \n");
        printf("[2] ---- construção do IDF; \n");
        printf("[3] ---- impressão do IDF; \n");
        printf("[4] ---- Realizar busca de documentos ordenados pelo TF-IDF; \n");
        printf("[0] ---- encerrar o programa; \n");

        printf("Digite sua opção: ");
        wscanf(L"%d",&command);
        fflush(stdin);

        if(command == 0){
            printf("--- Fim do programa ---\n");
            break;
        }else if(command  == 4){
            int temp_4x;
            printf("Digite quantidade de 0 a 4 para termos para o calculo do TF-IDF:");
            wscanf(L"%d",&temp_4x);
            fflush(stdin);
            for(int i =0; i < temp_4x; i++){
                printf("Digite o %do termo:\n",i+1);
                wscanf(L"%ls",input_words[i]);
            }

            for(int i = 0; i < temp_doc_number; i++){
                printf("TF do doc %d = %.2f\n",i,TF_IDF(input_words, temp_4x, &pat, temp_doc_number, i, temp_exp[i]));
            }

        }
    } 






    wprintf(L"%ls\n",input_words[0]);
    wprintf(L"%ls\n",input_words[1]);


    //wcscpy(input_words[0], L"quer");wcscpy(input_words[1], L"todos");


    patricia_update(&pat, L"quer", 1);
    patricia_update(&pat, L"quer", 1);
    patricia_update(&pat, L"quer", 1);

    patricia_update(&pat, L"quer", 2);

    
    patricia_update(&pat, L"todos", 2);
    patricia_update(&pat, L"todos", 2);
    
    
    
    int top = patricia_pairs(pat.root, input_words[0], &pairs);
    printf("doc %d, oc %d\n", pairs[0].file_id, pairs[0].nr);

    top = patricia_pairs(pat.root, input_words[1], &pairs);
    printf("doc %d, oc %d\n", pairs[0].file_id, pairs[0].nr);

    
    float doc0 = TF_IDF(input_words, 2, &pat, 2, 0, 7);

    printf("TF do doc 1 = %.2f\n",doc0);
    printf("TF do doc 2 = %.2f\n",TF_IDF(input_words, 2, &pat, 2, 1, 8));

    patricia_free(&pat);

    return 0;
}
