/* Main.c
 * author: xero7689
 * email: volleyp7689@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "patternQueue.h"
#include "strMatch.h"
#include "output.h"

#define DEBUG

char* create_buf(FILE*);
int length(FILE*);

struct thread_params{
    Node* pq;
    char* fb;
    FILE* op;
};

void thread_to_work(void*);

int main(int argc, char* argv[]){
    if(argc > 2){
        printf("Too many argumnet to supply.\n");
        return 1;
    }

    // Initialize Thread Pool
    int num_of_thread = strtol(argv[1], NULL, 10);
    pthread_t thread_array[num_of_thread];

    FILE* file_ptr;
    FILE* pattern_ptr;
    FILE* output;

    char* file_buf;
    char* pattern_buf;

    // Open file.
    file_ptr = fopen("test.txt", "r");
    pattern_ptr = fopen("term.txt", "r");
    output = fopen("output.txt", "w+");

    // Check if file doesn't exist.
    if(file_ptr == NULL || pattern_ptr == NULL){
        perror("Error while openning the file.");
        exit(EXIT_FAILURE);
    }

    // Create Buffer for test.txt and term.txt.
    file_buf = create_buf(file_ptr);
    pattern_buf = create_buf(pattern_ptr);

    // buf_length
    //printf("file_buf length:%lu\n", strlen(file_buf));
    //printf("pattern_buf length:%lu\n", strlen(pattern_buf));

    // Create PatternQueue
    Node* pq = patternQueue(pattern_buf);
    /*
    while(!pq_isEmpty(patterns)){
        printf("pql:%d\n", pq_length(patterns));
        pc = mpMatch(file_buf, pq_pop(patterns));
    }
    */

    struct thread_params tp;
    tp.pq = pq;
    tp.fb = file_buf;
    tp.op = output;


    pthread_t threads[num_of_thread];
    for(int i = 0; i < num_of_thread; i++){
        pthread_create(&threads[i], NULL, thread_to_work, &tp);
        pthread_join(threads[i], NULL);
    }
    //thread_to_work(&tp);

    // File Close
    fclose(file_ptr);
    fclose(pattern_ptr);
    fclose(output);

    return 0;
}

void thread_to_work(void* par){
    struct thread_params* tp = par;
    struct pattern_chain* pc;
    while(!pq_isEmpty(tp->pq)){
        pc = mpMatch(tp->fb, pq_pop(tp->pq));
        printf("%s\n", pc->pattern);
        fprintf(tp->op, "%s: (%d)", pc->pattern, pcLength(pc));
        while(!isEmpty(pc)){
            fprintf(tp->op, " %d", pop_index(pc));
        }
        fprintf(tp->op, "\n");
    }
    printf("thread finish!\n");
    pthread_exit(0);
}

int length(FILE* fp){
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return size;
}

char* create_buf(FILE* src){
    char* src_buf = malloc(length(src)*sizeof(char));
    char ch;
    int i = 0;
    while((ch = fgetc(src)) != EOF)
        src_buf[i++] = ch;
    return src_buf;
}
