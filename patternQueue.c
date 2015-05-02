#include "patternQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Node* patternQueue(const char* pattern_buf){
    Node* root = malloc(sizeof(Node));
    Node* current = root;
    Node* next;
    unsigned long pattern_length = strlen(pattern_buf);
    //int pattern_index = 0;
    int start = 0;
    int end = 0;
    int index = 0;
    char* tmp_pattern;

    // Read Line
    while(pattern_buf[end] != '\0'){
        for(end;;end++){
            if(pattern_buf[end] == '\r' && pattern_buf[end+1] == '\n')
                break;
        }
        tmp_pattern = malloc(((end-start)+1)*sizeof(char));
        for(int i = 0; i < (end-start); i++){
           tmp_pattern[i] = pattern_buf[start+i];
        }

        // fix tmp_pattern
//#define DEBUG
#ifdef DEBUG
        printf("%s, start:%d, end:%d, length:%lu\n", tmp_pattern, start, end, strlen(tmp_pattern));
#endif
        // Save to queue
        current->str = tmp_pattern;
        next = malloc(sizeof(Node));
        current->next = next;
        current = next;

        // Move Index
        end = end + 2;
        start = end;
    }
    current->str = "";
    current->next = NULL;
    return root;
}

int pq_length(const Node* pq){
    int length = 0;
    const Node* tmp = pq;
    while(tmp->next != NULL){
        tmp = tmp->next;
        length++;
    }
    return length++;
}

/*
char* pq_pop(Node* pq){
    Node* tmp;
    char* str;
    tmp = pq;
    printf("root:%x\n", pq);
    pq = pq->next;
    printf("root:%x\n", pq);
    str= tmp->str;
    free(tmp);
    return str;
}

int pq_push(Node* pq, const char* str){
    Node* tmp = pq;
    Node* newNode = malloc(sizeof(Node));
    newNode->str = str;
    newNode->next = NULL;
    while(tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = newNode;
    return 1;
}
*/
