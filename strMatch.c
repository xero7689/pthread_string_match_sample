#include "strMatch.h"
#include <string.h>
#include <stdio.h>

/* Brute Force Algorithm
struct pattern_chain* bfMatch(char* text, char* pattern){
    int tl = (int)strlen(text);
    int pl = (int)strlen(pattern);
    int total = 0;
    //int* result = malloc(100*sizeof(int));
    
    for(int i = 0; i < tl; ++i){
        if(i + pl > tl) break;
        int match = 1;
        for(int j = 0; j < pl; ++j){
            if(text[i+j] != pattern[j]){
                match = 0;
                break;
            }
        }
        if(match){
            total++;
            //printf("%d\t ", i);
        }
    }
    printf("Total: %d\n", total);
}
*/

// Moris Pratt Algorithm
struct pattern_chain* mpMatch(const char* text, const char* pattern){
    int tl = strlen(text);
    int pl = strlen(pattern);
    int failure[100];
    int total = 0;
    patternChain* pc = newPatternChain(pattern);

    if (pl > tl) return 0;

    for(int i = 1, j = failure[0] = -1; i < pl; i++){
        while (j >= 0 && pattern[j+1] != pattern[i]) j = failure[j];
        if (pattern[j+1] == pattern[i]) j++;
        failure[i] = j;
    }

    for(int i = 0, j = -1; i < tl; i++){
        while(j >= 0 && pattern[j+1] != text[i]) j = failure[j];
        if(pattern[j+1] == text[i]) j++;

        if(j == pl - 1){
#ifdef DEBUG
            printf("%d\t", i-pl+1);
#endif
            add_index(pc, (int)(i-pl+1));
            j = failure[j];
        }
    }
    return pc;
}
