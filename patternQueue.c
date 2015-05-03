#include "patternQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Node* patternQueue(const char* pattern_buf){
    Node* root = malloc(sizeof(Node));
    Node* current = root;
    Node* next;
    //unsigned long pattern_length = strlen(pattern_buf);
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
    current = 0;

    // This is the real root of queue.
    Node* head = malloc(sizeof(Node));
    head->str="";
    head->next = root;

    return head;
}

int pq_length(const Node* pq){
    int length = 0;
    const Node* tmp = pq->next;
    while(tmp->next != 0){
        tmp = tmp->next;
        length++;
    }
    return length++;
}

int pq_isEmpty(const Node* pq){
    Node* tmp = pq->next->next;
    //printf("%d\n", tmp);
    if(tmp != 0)
        return 0;
    else
        return 1;
}


char* pq_pop(Node* pq){
    Node* tmp;
    char* str;
    tmp = pq->next;
    str= tmp->str;
    pq->next = tmp->next;
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
