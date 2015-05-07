#ifndef OUTPUT_H
#define OUTPUT_H

typedef struct index_chain{
    int index;
    struct index_chain* next;
}indexChain;

typedef struct pattern_chain{
    char* pattern;
    struct index_chain* root;
}patternChain;

// String combination
char* concat(char* str1, char* str2);

struct pattern_chain* newPatternChain(const char* pattern);
int isEmpty(struct pattern_chain* pc);
int pcLength(struct pattern_chain* pc);
void add_index(struct pattern_chain* pc, int index);
int pop_index(struct pattern_chain* pc);
char* toString(struct pattern_chain* pc);

#endif
