#include <stdio.h>
#include "patricia.h"

int main(void) {
    Patricia pat;
    patricia_init(&pat, 5);

    patricia_update(&pat, L"vitória", 100);

    patricia_update(&pat, L"viçosa", 100);
    patricia_update(&pat, L"viçosa", 100);
    patricia_update(&pat, L"viçosa", 100);
    patricia_update(&pat, L"viçosa", 200);
    patricia_update(&pat, L"viçosa", 200);

    patricia_update(&pat, L"órgão", 300);
    patricia_update(&pat, L"órgão", 300);
    patricia_update(&pat, L"órgão", 400);
    patricia_update(&pat, L"órgão", 400);
    patricia_update(&pat, L"órgão", 400);
    patricia_update(&pat, L"órgão", 400);

    patricia_update(&pat, L"pinguim", 500);
    patricia_update(&pat, L"pinguim", 500);
    patricia_update(&pat, L"pinguim", 500);
    patricia_update(&pat, L"pinguim", 500);
    patricia_update(&pat, L"pinguim", 500);
    patricia_update(&pat, L"pinguim", 500);
    patricia_update(&pat, L"pinguim", 500);
    patricia_update(&pat, L"pinguim", 500);
    patricia_update(&pat, L"pinguim", 500);

    Pair *pairs;
    int top = patricia_pairs(pat.root, L"viçosa", &pairs);
    for(int i = 0; i < top; ++i)
        printf("No arquivo %d, viçosa aparece %d vezes\n", 
            pairs[i].file_id, pairs[i].nr);

    top = patricia_pairs(pat.root, L"vitória", &pairs);
    for(int i = 0; i < top; ++i)
        printf("No arquivo %d, vitória aparece %d vezes\n", 
            pairs[i].file_id, pairs[i].nr);

    top = patricia_pairs(pat.root, L"órgão", &pairs);
    for(int i = 0; i < top; ++i)
        printf("No arquivo %d, órgão aparece %d vezes\n", 
            pairs[i].file_id, pairs[i].nr);

    top = patricia_pairs(pat.root, L"pinguim", &pairs);
    for(int i = 0; i < top; ++i)
        printf("No arquivo %d, pinguim aparece %d vezes\n", 
            pairs[i].file_id, pairs[i].nr);

    patricia_free(&pat);
    return 0;
}
