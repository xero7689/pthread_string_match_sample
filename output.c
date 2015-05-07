#include "output.h"
#include <stdlib.h>

int isEmpty(struct pattern_chain* pc){
    if(pc->root != 0){
        return 0;
    }
    else{
        return 1;
    }
}

int pcLength(struct pattern_chain *pc){
    struct index_chain* cur;
    int total = 0;
    cur = pc->root;
    while(cur->next != 0){
        cur = cur->next;
        total++;
    }
    return total;
}

patternChain* newPatternChain(const char* pattern){
    patternChain* new = malloc(sizeof(patternChain));
    new->pattern = pattern;
    new->root = 0;
    return new;
}

void add_index(struct pattern_chain* pc, int index){
    struct index_chain* cur;
    if(pc->root == 0){
        cur = malloc(sizeof(cur));
        cur->index = index;
        cur->next = 0;
        pc->root = cur;
        return;
    }
    cur = pc->root;
    while(cur->next != 0)
        cur = cur->next;
    struct index_chain* tmp = malloc(sizeof(cur));
    tmp->index = index;
    tmp->next = 0;
    cur->next = tmp;
}

int pop_index(struct pattern_chain* pc){
    indexChain* cur;
    indexChain* next;
    int index;

    cur = pc->root;
    index = cur->index;
    next = cur->next;
    pc->root = next;
    //free(cur);  // A double free bug
    return index;
}

char* toString(struct pattern_chain* pc){
    char* str;
    struct index_chain* cur;
    cur = pc->root;
    while(cur->next != 0)
        cur = cur->next;
    return str;
}
